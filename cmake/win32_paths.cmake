# ���� �������� ���������� � ������ ������ ���������� � ���������
# ��� �������� �� ������� cg_course_examples.

# �� ��������� Windows ��� �����������:
#   - ��������� ���������� ��������� SDK2015 �� ���� ����������, ����� ��� ��������� �� �������� ������� � ������������
#   - � �������� �������� � ������������ ������� � ������� ����������� ���������� (SDL2, boost � �.�.), ����� ��������������� ���� ���������

if(WIN32)
	if (CMAKE_BUILD_TYPE EQUAL "Debug")
		set(CG_DEBUG_SUFFIX "d")
	else (CMAKE_BUILD_TYPE EQUAL "Debug")
		set(CG_DEBUG_SUFFIX "")
	endif (CMAKE_BUILD_TYPE EQUAL "Debug")

	# ���� ������ ������������ ������
	set(CG_WIN32_INCLUDE_PATHS
		 "$ENV{SDK2015}/boost/include"
		 "$ENV{SDK2015}/SDL2/include"
		 "$ENV{SDK2015}/glm"
		 "$ENV{SDK2015}/assimp-3.3.1/include/"
		 "$ENV{SDK2015}/glew/include")
 
 # ���� ������ ���������
 # ���������� ���� $(VAR) �������� ����������� � CMake,
 # �� �������� Visual Studio
 
 # ��� 32-������ ��������
	if( CMAKE_SIZEOF_VOID_P EQUAL 4)

		set(CG_WIN32_LINKER_PATHS
			 "$ENV{SDK2015}/boost/lib"
			 "$ENV{SDK2015}/SDL2/lib/x86"
			 "$ENV{SDK2015}/assimp-3.3.1/lib/${CMAKE_BUILD_TYPE}"
			 "$ENV{SDK2015}/glew/lib/$(Configuration)/Win32")
 
 # ��� 64-������ ��������
	else( CMAKE_SIZEOF_VOID_P EQUAL 4)
	
	endif( CMAKE_SIZEOF_VOID_P EQUAL 4)
 
endif(WIN32)
