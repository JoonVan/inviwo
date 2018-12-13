def cmd(stageName, dirName, env = [], fun) {
    stage(stageName) {
        dir(dirName) {
            nicelog(env) {
                fun()
            }
        }
    }
}

return this