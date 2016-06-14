#!/bin/bash

# Dependencies:
#  - Python
#  - Python URLlib
#  - Python JS Beautifier (https://github.com/beautify-web/js-beautify)
#  - xmllint (package libxml2-utils)
#  - curl (for those who stil don't have it under Linux xD)

set -euo pipefail

EXPERIMENTAL=false

while test $# -gt 0
do
	case "$1" in
		-e)
			EXPERIMENTAL=true
		;;

		-h|--help)
			echo "Returns the direct stream link for a flashx.tv video"
			echo ""
			echo "Usage: $0 [-e] [ http://www.flashx.tv/<id>.html | <id> ]"
			echo "  -e: Returns the URL with fixed filename (experimental)"
			exit 0
		;;

		*)
			if [[ "$1" =~ ^http://www.flashx.tv/[a-zA-Z0-9]+.html$ ]]; then
				BASE_URL="$1"
			elif [[ "$1" =~ ^[a-zA-Z0-9]+$ ]]; then
				BASE_URL="http://www.flashx.tv/$1.html"
			else
				echo "Invalid URL or FlashX ID"
				exit 1
			fi
		;;
	esac
	shift
done

set +e
	COOKIE_FILE="/tmp/flashx_cookies_`cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 32 | head -n 1`"
set -e

ID=$(basename "$BASE_URL" .html)
USER_AGENT="User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:33.0) Gecko/20100101 Firefox/33.0"

INIT=$(curl "$BASE_URL" --cookie $COOKIE_FILE --cookie-jar $COOKIE_FILE -H "$USER_AGENT" 2> /dev/null)

HASH=$(echo "$INIT" | xmllint --html --xpath "string(//form[@action='http://www.flashx.tv/dl?${ID}']/input[@name='hash']/@value)" -)
FNAME=$(echo "$INIT" | xmllint --html --xpath "string(//form[@action='http://www.flashx.tv/dl?${ID}']/input[@name='fname']/@value)" -)
FILE_ID=$(echo "$INIT" | grep -Eo "^.\.cookie.*file_id.*" | grep -Eo "'[0-9]+'" | grep -Eo "[0-9]+")
AFF=$(echo "$INIT" | grep -Eo "^.\.cookie.*aff.*" | grep -Eo "'[0-9]+'" | grep -Eo "[0-9]+")

sleep 6

PAYLOAD="op=download1&usr_login=&id=${ID}&fname=${FNAME}&referer=&hash=${HASH}&imhuman=Proceed+to+video"
PAGE=$(curl -X POST "http://www.flashx.tv/dl?${ID}" --cookie $COOKIE_FILE --cookie-jar $COOKIE_FILE -d "$PAYLOAD" --cookie "ab_noticed=true; aff=$AFF; file_id=$FILE_ID; ref_url=http://www.flashx.tv/${ID}.html" -H "$USER_AGENT" 2> /dev/null)
JS=$(echo "$PAGE" | grep -Eo "eval\(function\(.*")

rm $COOKIE_FILE

URL=$(echo "$JS" | python -c "import sys; import jsbeautifier; print(jsbeautifier.beautify(sys.stdin.read()))" | grep -Eo "file:\s*\"http.*" | grep -Eo "http.+?\.mp4")

if $EXPERIMENTAL ; then
	BASE=$(dirname $URL)
	EXT="${URL##*.}"
	FILENAME=$(python -c "import urllib; print urllib.quote('$FNAME')")
	echo "$BASE/$FILENAME.$EXT"
else
	echo $URL
fi


