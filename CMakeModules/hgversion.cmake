set(CMAKE_MODULE_PATH ${TOP_SOURCE_DIR}/CMakeModules)
find_package(Mercurial)
if (MERCURIAL_FOUND)
  MERCURIAL_HG_INFO(${TOP_SOURCE_DIR} TOOLS)
  MESSAGE("Current revision is ${TOOLS_HG_ID}")
  set (TOOLS_HG_ID "hgid: ${TOOLS_HG_ID}")
else(MERCURIAL_FOUND)
  set (TOOLS_HG_ID)
endif(MERCURIAL_FOUND)
set (HG_HEADER "hgversion.h")
set (NEW_HG_HEADER "new_hgversion.h")
file(WRITE ${NEW_HG_HEADER} "static const std::string hgversion = \"${TOOLS_HG_ID}\";\n")
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${NEW_HG_HEADER} ${HG_HEADER})
execute_process(COMMAND ${CMAKE_COMMAND} -E remove ${NEW_HG_HEADER})
