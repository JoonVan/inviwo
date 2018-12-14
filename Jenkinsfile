node {
    try {
        stage('Fetch') { 
            dir('inviwo') {
                checkout scm
                sh 'git submodule sync --recursive' // needed when a submodule has a new url  
                sh 'git submodule update --init --recursive'
            }
        }

        def rootDir = pwd()
        def util = load "${rootDir}/inviwo/tools/jenkins/util.groovy"
        
        properties(util.defaultProperties())

        stage('Build') {
            if (params['Clean Build']) {
                echo "Clean build, removing build folder"
                sh "rm -r build"
            }
            dir('build') {
                util.log {
                    sh """
                        ccache -z # reset ccache statistics
                        # tell ccache where the project root is
                        export CPATH=`pwd`
                        export CCACHE_BASEDIR=`readlink -f \${CPATH}/..`
                        
                        cmake -G \"Ninja\" -LA \
                              -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
                              -DCMAKE_BUILD_TYPE=${params['Build Type']} \
                              -DOpenCL_LIBRARY=/usr/local/cuda/lib64/libOpenCL.so  \
                              -DOpenCL_INCLUDE_DIR=/usr/local/cuda/include/ \
                              -DCMAKE_PREFIX_PATH=/opt/Qt/5.6/gcc_64 \
                              -DIVW_CMAKE_DEBUG=ON \
                              -DIVW_DOXYGEN_PROJECT=ON \
                              -DBUILD_SHARED_LIBS=ON \
                              -DIVW_MODULE_GLFW=ON \
                              -DIVW_TINY_GLFW_APPLICATION=ON \
                              -DIVW_TINY_QT_APPLICATION=ON \
                              -DIVW_MODULE_ABUFFERGL=ON \
                              -DIVW_MODULE_ANIMATION=ON \
                              -DIVW_MODULE_ANIMATIONQT=ON \
                              -DIVW_MODULE_PLOTTING=ON \
                              -DIVW_MODULE_PLOTTINGGL=ON \
                              -DIVW_MODULE_POSTPROCESSING=ON \
                              -DIVW_MODULE_USERINTERFACEGL=ON \
                              -DIVW_MODULE_HDF5=ON \
                              -DIVW_MODULE_DISCRETEDATA=ON \
                              -DIVW_UNITTESTS=ON \
                              -DIVW_UNITTESTS_RUN_ON_BUILD=OFF \
                              -DIVW_INTEGRATION_TESTS=ON \
                              -DIVW_RUNTIME_MODULE_LOADING=ON \
                              ../inviwo

                        ninja

                        ccache -s # print ccache statistics
                    """
                }
            }
        }

        def display = 0       

        util.unittest(display)
        
        util.integrationtest(display)        
        
        util.regression(currentBuild, display, env)

        util.cmd('Copyright Check', 'inviwo') {
            sh 'python3 tools/refactoring/check-copyright.py .'
        }
        
        util.cmd('Doxygen', 'build', ['DISPLAY=:' + display]) {
            sh 'ninja DOXY-ALL'
        }
        
        util.publish()

        currentBuild.result = 'SUCCESS'
    } catch (e) {
        currentBuild.result = 'FAILURE'
        throw e
    } finally {
        util.slack(currentBuild, env)
    }
}
