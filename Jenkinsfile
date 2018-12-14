node {
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

    try {
        stage('Build') {
            if (params['Clean Build']) {
                echo "Clean build, removing build folder"
                sh "rm -r build"
            }
            def opts = [
                "CMAKE_CXX_COMPILER_LAUNCHER" : "ccache",
                "CMAKE_BUILD_TYPE" : params['Build Type'],
                "OpenCL_LIBRARY" : "/usr/local/cuda/lib64/libOpenCL.so",
                "OpenCL_INCLUDE_DIR" : "/usr/local/cuda/include/",
                "CMAKE_PREFIX_PATH" : "/opt/Qt/5.6/gcc_64",
                "IVW_CMAKE_DEBUG" : "ON",
                "IVW_DOXYGEN_PROJECT" : "ON",
                "BUILD_SHARED_LIBS" : "ON",
                "IVW_TINY_GLFW_APPLICATION" : "ON",
                "IVW_TINY_QT_APPLICATION" : "ON",
                "IVW_UNITTESTS" : "ON",
                "IVW_UNITTESTS_RUN_ON_BUILD" : "OFF",
                "IVW_INTEGRATION_TESTS" : "ON",
                "IVW_RUNTIME_MODULE_LOADING" : "ON"
            ]
            def onModules = ["ABUFFERGL" , "DISCRETEDATA", "GLFW", "HDF5"]
            util.build(opts, onModules)
        }

        def display = 0       
        util.unittest(display)
        util.integrationtest(display)        
        util.regression(currentBuild, display, env)
        util.copyright()
        util.doxygen(display)       
        util.publish()

        currentBuild.result = 'SUCCESS'
    } catch (e) {
        currentBuild.result = 'FAILURE'
        throw e
    } finally {
        util.slack(currentBuild, env)
    }
}
