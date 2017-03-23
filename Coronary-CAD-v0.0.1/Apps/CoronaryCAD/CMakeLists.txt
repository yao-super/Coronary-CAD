set (FOO_SRCS
	 CoronaryCAD.cpp

)
# Generate the executable application for the current poject
ADD_EXECUTABLE (CoronaryCAD 
				${FOO_SRCS}
#				${FOO_MOC_H}
)

# Add the link-libraries for the current poject
TARGET_LINK_LIBRARIES (CoronaryCAD 
					   ${QT_LIBRARIES}
					   vacMainInterface
					   
)
SET_TARGET_PROPERTIES(CoronaryCAD PROPERTIES FOLDER "Apps")
# The installation path for the application
INSTALL_TARGETS (/bin CoronaryCAD )

 