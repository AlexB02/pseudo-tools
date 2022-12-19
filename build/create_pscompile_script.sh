#!/bin/bash
touch out/pscompile
echo "#!/bin/bash\njava -jar $(pwd)/out/ptp-latest.jar \$@" > out/pscompile
chmod +x out/pscompile
