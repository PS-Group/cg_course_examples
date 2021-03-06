#include "stdafx.h"
#include "WindowClient.h"
#include "includes/opengl-common.hpp"
#include <glm/gtc/matrix_transform.hpp>

using glm::mat4;
using glm::vec3;
using glm::vec4;

namespace
{
const glm::vec3 CAMERA_EYE = { 0, 1, 2 };
const glm::vec4 SUNLIGHT_POSITION = {0, 0, 0, 1};
const glm::vec4 WHITE_RGBA = {1, 1, 1, 1};
const glm::vec4 FADED_WHITE_RGBA = {0.3f, 0.3f, 0.3f, 1.0f};

void SetupOpenGLState()
{
    // включаем механизмы трёхмерного мира.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
}

glm::mat4 MakeProjectionMatrix(const glm::ivec2 &size)
{
    // Матрица перспективного преобразования вычисляется функцией
    // glm::perspective, принимающей угол обзора, соотношение ширины
    // и высоты окна, расстояния до ближней и дальней плоскостей отсечения.
    const float fieldOfView = glm::radians(70.f);
    const float aspect = float(size.x) / float(size.y);
    const float zNear = 0.01f;
    const float zFar = 100.f;

    return glm::perspective(fieldOfView, aspect, zNear, zFar);
}
}

CWindowClient::CWindowClient(CWindow &window)
    : CAbstractWindowClient(window)
    , m_defaultVAO(CArrayObject::do_bind_tag())
    , m_keplerSystem(m_timeController)
    , m_rotationSystem(m_timeController)
    , m_mouseGrabber(window)
    , m_camera(CAMERA_EYE)
{
    const vec4 BLACK_RGBA = {0, 0, 0, 1};
    window.SetBackgroundColor(BLACK_RGBA);
    SetupOpenGLState();

    m_renderSystem.SetupLight0(SUNLIGHT_POSITION, WHITE_RGBA, FADED_WHITE_RGBA);

    CSceneLoader loader(m_world);
    loader.LoadScene("res/solar_system/solar_system_2012.json");
    loader.LoadSkybox("res/solar_system/skybox.plist");

    // Добавляем систему, отвечающую за изменение положения планет
    //  согласно их орбитам и прошедшему времени по законам Кеплера.
    m_world.addSystem(m_keplerSystem);

    // Добавляем систему, выполняющую вращение тел вокруг своих осей.
    m_world.addSystem(m_rotationSystem);

    // Добавляем систему, отвечающую за обновление систем частиц
    m_world.addSystem(m_particleUpdateSystem);

    // Добавляем систему, отвечающую за рендеринг планет.
    m_world.addSystem(m_renderSystem);

    // Добавляем систему, отвечающую за рендеринг систем частиц
    m_world.addSystem(m_particleRenderSystem);

    // После активации новых сущностей или деактивации,
    //  а при добавления новых систем следует
    //  вызывать refresh() у мира.
    m_world.refresh();
}

void CWindowClient::OnUpdate(float deltaSeconds)
{
    m_camera.Update(deltaSeconds);
    m_timeController.Update(deltaSeconds);
    m_keplerSystem.Update();
    m_rotationSystem.Update();
    m_particleUpdateSystem.Update(deltaSeconds);
}

void CWindowClient::OnDraw()
{
    const glm::ivec2 windowSize = GetWindow().GetWindowSize();

    const mat4 view = m_camera.GetViewMat4();
    const mat4 projection = MakeProjectionMatrix(windowSize);

    glViewport(0, 0, windowSize.x, windowSize.y);
    m_renderSystem.Render(view, projection);
    m_particleRenderSystem.Render(view, projection);
}

bool CWindowClient::OnKeyDown(const SDL_KeyboardEvent &event)
{
    return m_camera.OnKeyDown(event);
}

bool CWindowClient::OnKeyUp(const SDL_KeyboardEvent &event)
{
    return m_camera.OnKeyUp(event);
}

bool CWindowClient::OnMousePress(const SDL_MouseButtonEvent &event)
{
    return m_camera.OnMousePress(event);
}

bool CWindowClient::OnMouseMotion(const SDL_MouseMotionEvent &event)
{
    return m_mouseGrabber.OnMouseMotion(event)
            || m_camera.OnMouseMotion(event);
}

bool CWindowClient::OnMouseUp(const SDL_MouseButtonEvent &event)
{
    return m_camera.OnMouseUp(event);
}
