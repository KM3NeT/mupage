if [ ! -n "${ROOTSYS}" ]; then
  module load root
fi
export LD_LIBRARY_PATH=$(pwd)/../../lib:$LD_LIBRARY_PATH
