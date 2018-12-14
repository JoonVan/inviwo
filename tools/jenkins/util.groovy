@NonCPS
def getChangeString(build) {
    def MAX_MSG_LEN = 100
    def changeString = ""

    echo "Gathering SCM changes"
    def changeLogSets = build.rawBuild.changeSets
    for (int i = 0; i < changeLogSets.size(); i++) {
        def entries = changeLogSets[i].items
        for (int j = 0; j < entries.length; j++) {
            def entry = entries[j]
            truncated_msg = entry.msg.take(MAX_MSG_LEN)
            changeString += "${new Date(entry.timestamp).format("yyyy-MM-dd HH:mm:ss")} "
            changeString += "[${entry.commitId.take(8)}] ${entry.author}: ${truncated_msg}\n"
        }
    }

    if (!changeString) {
        changeString = " - No new changes"
    }
    return changeString
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

def slack(build, env) {
    stage('Slack') {
        echo "result: ${build.result}"
        def res2color = ['SUCCESS' : 'good', 'UNSTABLE' : 'warning' , 'FAILURE' : 'danger' ]
        def color = res2color.containsKey(build.result) ? res2color[build.result] : 'warning'
        slackSend(
            color: color, 
            channel: "#jenkins-branch-pr", 
            message: "Inviwo branch: ${env.BRANCH_NAME}\n" + \
                     "Status: ${build.result}\n" + \
                     "Job: ${env.BUILD_URL} \n" + \
                     "Regression: ${env.JOB_URL}Regression_Report/\n" + \
                     "Changes: " + getChangeString(build) 
        )
    }
}



return this
