
find_path(MBEDTLS_INCLUDE_DIR ssl_ciphersuites.h ssl.h ssl_ciphersuites.h
    HINTS $ENV{MBEDTLS_DIR} $ENV{MBEDTLS_DIR}
    PATH_SUFFIXES include/mbedtls include
    PATHS ~/Library/Frameworks /Library/Frameworks /usr/local /usr /sw /opt/local /opt/csw /opt
)
find_library(MBEDTLS_LIBRARY
    NAMES mbedtls mbedtls
    HINTS $ENV{MBEDTLS_DIR} $ENV{MBEDTLS_DIR}
    PATH_SUFFIXES lib64 lib
    PATHS ~/Library/Frameworks /Library/Frameworks /usr/local /usr /sw /opt/local /opt/csw /opt
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(mbedtls DEFAULT_MSG MBEDTLS_LIBRARY MBEDTLS_INCLUDE_DIR)

if(MBEDTLS_FOUND)
    set(MBEDTLS_INCLUDE_DIRS "${MBEDTLS_INCLUDE_DIR}" CACHE STRING "" FORCE)
    set(MBEDTLS_LIBRARIES "${MBEDTLS_LIBRARY}" CACHE STRING "" FORCE)
endif(MBEDTLS_FOUND)

mark_as_advanced(MBEDTLS_INCLUDE_DIR MBEDTLS_INCLUDE_DIRS)
mark_as_advanced(MBEDTLS_LIBRARY MBEDTLS_LIBRARIES)
