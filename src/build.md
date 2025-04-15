If you just want to undo the commit, but keep your code as uncommitted changes:>> git reset --soft HEAD~1

cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cp build/compile_commands.json ~/code/projects/pktcore/
