#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/bingheng-xie/ur_us/src/easy_handeye/rqt_easy_handeye"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/bingheng-xie/ur_us/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/bingheng-xie/ur_us/install/lib/python2.7/dist-packages:/home/bingheng-xie/ur_us/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/bingheng-xie/ur_us/build" \
    "/usr/bin/python2" \
    "/home/bingheng-xie/ur_us/src/easy_handeye/rqt_easy_handeye/setup.py" \
    egg_info --egg-base /home/bingheng-xie/ur_us/build/easy_handeye/rqt_easy_handeye \
    build --build-base "/home/bingheng-xie/ur_us/build/easy_handeye/rqt_easy_handeye" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/bingheng-xie/ur_us/install" --install-scripts="/home/bingheng-xie/ur_us/install/bin"
