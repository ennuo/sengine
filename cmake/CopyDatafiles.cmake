add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E rm -R -f
	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/datafiles"
	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/datafiles"
	"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")