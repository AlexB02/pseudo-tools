#!/bin/bash
touch build/out/pscompile
echo "#!/bin/bash\njava -jar $(pwd)/build/out/ptp-latest.jar \$@" > build/out/pscompile
chmod +x build/out/pscompile
