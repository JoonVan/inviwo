def log(env = [], fun) {
    withEnv(['TERM=xterm'] + env) {
        ansiColor {
            timestamps {
                fun()
            }
        }
    }
}

def cmd(stageName, dirName, env = [], fun) {
    stage(stageName) {
        dir(dirName) {
            log(env) {
                fun()
            }
        }
    }
}

def defaultProperties() {
    return [
            parameters([
                booleanParam(
                    defaultValue: false, 
                    description: 'Do a clean build', 
                    name: 'Clean Build'
                ),
                choice(
                    choices: "Release\nDebug\nMinSizeRel\nRelWithDebInfo\n", // The first will be default
                    description: 'Select build configuration', 
                    name: 'Build Type'
                )
            ]),
            pipelineTriggers([
                [$class: 'GitHubPushTrigger']
            ])
        ]
}

return this
