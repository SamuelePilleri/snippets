#!/bin/bash

set -euo pipefail

fs() (
	# Returns char identifing file type as it appears in `ls -la`.
	# If argument is file returns `f`
	function get_element_type {
		local char=$(ls -lad "$1" | head -c 1)
		if [ "$char" == "-" ]; then
			char="f"
		fi
		echo $char
	}

	# Will return true only if given argument is a folder. If it is a link
	# to a folder, it will return false.
	# Eg.
	#   mydir (folder)
	#   mylnk (link to mydir)
	#  If argument is mylnk it will return false (link)
	#  If argument is mylnk/ it will return true (resolved)
	function isdir {
		# Watch out!
		# In bash 0 is true and 1 is false
		local t=$(get_element_type "$1")
		if [ "$t" == "d" ]; then
			return 0
		else
			return 1
		fi
	}

	# See isdir, might wanna inline this
	function isdir_permissive {
		if [ -d "$1" ]; then
			return 0
		else
			return 1
		fi
	}

	# Finally a dirname which considers if I've actually already provided a
	# directory and not just removes the last part of filename.
	function dirname {
		if isdir "$1"; then
			echo "$1"
		else
			command dirname "$1"
		fi
	}

	path() (
		# Simply normalizes removing .. , . , // , etc.
		function normalize {

			# Thanks https://stackoverflow.com/a/31086901

			shopt -s extglob

			local path="$1"
			# get rid of /../ eg: /one/../two to /two
			path="${path//\/*([!\/])\/\.\./}"
			# get rid of /./ and //* eg: /one/.///two to /one/two
			path="${path//@(\/\.\/|\/+(\/))//}"
			# remove the last '/.'
			echo "${path%%/.}"

			shopt -u extglob
		}

		# Returns the absolute and normalized path. Symlinks are
		# followed.
		function resolve {
			realpath "$1"
		}

		# Same as `resolve` but symlinks are not followed.
		function resolve_nosym {
			realpath -s "$1"
		}

		function relative_to {
			local from=$(dirname "$1")
			local to=$(dirname "$2")

			realpath -s "$from" --relative-to "$to"
		}

		"$1" "${@:2}"
	)

	# We can call this dispatcher
	"$1" "${@:2}"
)

debug() (
	# Prints current stacktrace except for itself
	function print_stacktrace {
		for entry in ${FUNCNAME[@]:2}; do
			echo $entry
		done
	}

	"$1" "${@:2}"
)

"$1" "${@:2}"
