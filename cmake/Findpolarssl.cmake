
#find_package(PkgConfig)
#pkg_check_modules(PC_polarssl QUIET polarssl)
#set(POLARSSL_DEFINITIONS ${PC_polarssl_CFLAGS_OTHER})

find_path(POLARSSL_INCLUDE_DIR ssl_ciphersuites.h ssl.h ssl_ciphersuites.h
    HINTS $ENV{POLARSSL_DIR} $ENV{MBEDTLS_DIR}
    PATH_SUFFIXES include/polarssl include
    PATHS ~/Library/Frameworks /Library/Frameworks /usr/local /usr /sw /opt/local /opt/csw /opt
)
find_library(POLARSSL_LIBRARY
    NAMES polarssl mbedtls
    HINTS $ENV{POLARSSL_DIR} $ENV{MBEDTLS_DIR}
    PATH_SUFFIXES lib64 lib
    PATHS ~/Library/Frameworks /Library/Frameworks /usr/local /usr /sw /opt/local /opt/csw /opt
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(polarssl DEFAULT_MSG POLARSSL_LIBRARY POLARSSL_INCLUDE_DIR)

if(POLARSSL_FOUND)
    set(POLARSSL_INCLUDE_DIRS "${POLARSSL_INCLUDE_DIR}" CACHE STRING "" FORCE)
    set(POLARSSL_LIBRARIES "${POLARSSL_LIBRARY}" CACHE STRING "" FORCE)
endif(POLARSSL_FOUND)

mark_as_advanced(POLARSSL_INCLUDE_DIR POLARSSL_INCLUDE_DIRS)
mark_as_advanced(POLARSSL_LIBRARY POLARSSL_LIBRARIES)
