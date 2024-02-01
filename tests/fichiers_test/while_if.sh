# Script avec while et if
var=true

while $var ; do
    echo $var
    if $var; then
        var='false'
    fi
    echo $var
done
echo $var