#
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

# For QT moc
QT4_WRAP_CPP (FOO_MOC_OUTFILES vac3Dto2DTransformer.h)

# Create the uwiCMonitorTabSensors C++ library
ADD_LIBRARY (vac3Dto2DTransformer STATIC
			 ${ALL_SRCS} 
			 ${FOO_MOC_OUTFILES}
)

# Add the extra link-libraries
TARGET_LINK_LIBRARIES (vac3Dto2DTransformer
                       ${ITK_LIBRARIES}
					   ${QT_LIBRARIES}
)

SET_TARGET_PROPERTIES(vac3Dto2DTransformer PROPERTIES FOLDER "vacClass")

# The installation path for the link-library
INSTALL_TARGETS (/lib vac3Dto2DTransformer)

# The installation path for the include-files
INSTALL (FILES ${ALL_INCLUDES} DESTINATION include)

 