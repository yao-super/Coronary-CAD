
# Source files
#
# Here is where you can add the name of your local classes.
#

FILE (GLOB ALL_SOURCES "*.cpp" "*.c")
FILE (GLOB ALL_INCLUDES "*.hpp" "*.h")

SET (ALL_SRCS 
	 ${ALL_SOURCES}
	 ${ALL_INCLUDES}
)

# Create the uwiCMonitorTabSensors C++ library
ADD_LIBRARY (vacPointReader STATIC
			 ${ALL_SRCS} 
			 ${FOO_MOC_OUTFILES}
)

# Add the extra link-libraries
TARGET_LINK_LIBRARIES (vacPointReader 
                       ${ITK_LIBRARIES}
					   
)

SET_TARGET_PROPERTIES(vacPointReader PROPERTIES FOLDER "vacClass")

# The installation path for the link-library
INSTALL_TARGETS (/lib vacPointReader)

# The installation path for the include-files
INSTALL (FILES ${ALL_INCLUDES} DESTINATION include)