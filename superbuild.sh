#!/bin/bash
#
# Usage: superbuild.sh [ -egl | -osmesa ]
#

set -e

readonly paraview_version="5.8.0$1"
readonly image=lhofmann/paraview-superbuild:${paraview_version}
readonly container=build-paraview-vtk-template-${paraview_version}
readonly cwd="$(dirname "$(readlink -f "$0")")"

docker top ${container} >/dev/null 2>&1 || \
docker start ${container} >/dev/null 2>&1 || \
docker run -itd \
    --user "$(id -u ${USER}):$(id -g ${USER})" \
    --name ${container} \
    --volume="${cwd}:/mnt/source:ro" \
    ${image}

docker exec ${container} /usr/bin/scl enable devtoolset-8 -- cmake \
    -B/tmp/build \
    -H/mnt/source \
    -DCMAKE_INSTALL_LIBDIR=lib \
    -DCMAKE_BUILD_TYPE=Release \
    -DMili_INCLUDE_DIR=/home/paraview/buildbuildbuildbuildbuildbuildbuildbuildbuildbuildbuildbuildbuildbuild/install/include \
    -DCMAKE_INSTALL_PREFIX=/tmp/install/ParaView-${paraview_version}-MPI-Linux-Python3.7-64bit

docker exec ${container} cmake --build /tmp/build --target install -- -j4

mkdir -p package
docker cp "${container}:/tmp/install/." package/

# uncomment to disable incremental builds
# docker stop ${container}
# docker rm ${container}
