#!/usr/bin/env bash

# Launch ML tests with a chosen version of CP, to be sure that new deliveries will not break ML
# tests (unless when it is known and accepted)
#
# Current missing:
#       add the support of CP_VERSION="current" -- I need to ask Concrete team about that

set -e

function usage() {
    echo "$0: install selected CP and run CML tests"
    echo
    echo "About ML repository:"
    echo "--ml_branch <branch>              Specify the branch of ML repo (default is main)"
    echo
    echo "About CP version:"
    echo "--cp_version x.y.z                Install the chosen version of CP"
    echo "--last_nightly_cp_version         Install the last nightly of CP"
    echo
    echo "About the tests to run"
    echo "--use_cml_command_line            Use exactly commandline by ML team"
    echo
    echo "And the other options"
    echo "--quick_debug_of_the_script       Do not use, unless you debug the script"
    echo "--python <python_command_line>    Which python to use (eg, python3.9, default is python)"
    echo "--verbose                         Verbose"
    echo "--help                            Print this message"
    echo
}

# No real reason to change this. We could use tmp files, but then, when there are bugs, they would
# be harder to find
TMP_DIRECTORY="tmp_directory_for_cml_tests"
TMP_VENV=".venv_test_cml"

# Default is to use the Concrete current version, ie the one of the branch
CP_VERSION="current"

# Set to 1 only to debug quickly
DO_QUICK_SCRIPT_DEBUG=0

# Set to 1 to use exactly the pytest commandline of ML team. However, it's longer, since it runs
# the full test, while the one in this script exits at first problem
DO_USE_CML_PYTEST_COMMANDLINE=0

# Which python to use, to create the venv
PYTHON=python

# Variables of the script, don't change
IS_VERBOSE=0
ML_BRANCH="main"

while [ -n "$1" ]
do
   case "$1" in
        "--cp_version" )
            shift
            CP_VERSION="${1}"
            ;;

        "--last_nightly_cp_version" | "--last_cp_version" )
            CP_VERSION="last"
            ;;

        "--quick_debug_of_the_script" )
            DO_QUICK_SCRIPT_DEBUG=1
            ;;

        "--use_cml_command_line" )
            DO_USE_CML_PYTEST_COMMANDLINE=1
            ;;

        "--ml_branch" )
            shift
            ML_BRANCH="${1}"
            ;;

        "--python" )
            shift
            PYTHON="${1}"
            ;;

        "--verbose" )
            set -x
            IS_VERBOSE=1
            ;;

        *)
            echo "Unknown param : $1"
            usage
            exit 1
            ;;
   esac
   shift
done

# Directory for tests
echo
echo "Creating a temporary directory for CML tests"

if [ $DO_QUICK_SCRIPT_DEBUG -eq 0 ]
then
    rm -rf ${TMP_DIRECTORY}
    mkdir ${TMP_DIRECTORY}
else
    echo "    -- skipped during debug"
fi

cd ${TMP_DIRECTORY}

# Get repository
echo
echo "Getting CML repository"

if [ $DO_QUICK_SCRIPT_DEBUG -eq 0 ]
then
    git clone https://github.com/zama-ai/concrete-ml.git --branch ${ML_BRANCH}
else
    echo "    -- skipped during debug"
fi

cd concrete-ml

echo
echo "Used ML branch:"
git branch

# Install
echo
echo "Installing CML environment"

if [ $DO_QUICK_SCRIPT_DEBUG -eq 0 ]
then
    rm -rf ${TMP_VENV}
    ${PYTHON} -m venv ${TMP_VENV}
else
    echo "    -- skipped during debug"
fi

source ${TMP_VENV}/bin/activate

if [ $DO_QUICK_SCRIPT_DEBUG -eq 0 ]
then
    make sync_env
fi

echo
echo "Python which is used: "
which python

# Force CP version
echo
echo "Installing CP version"

if [ "$CP_VERSION" == "last" ]
then
    poetry run python -m pip install -U --pre "concrete-python" --no-deps
elif [ "$CP_VERSION" == "current" ]
then
    echo "Fix me: how to install the current CP, ie the one in the current directory"
    echo "That must be some: pip -e ."
    exit 255
else
    poetry run python -m pip install -U --pre "concrete-python==${CP_VERSION}" --no-deps
fi

INSTALLED_CP=`pip freeze | grep "concrete-python"`
echo
echo "Installed Concrete-Python: ${INSTALLED_CP}"

# Launch CML pytests
echo
echo "Launching CML tests"

if [ $DO_USE_CML_PYTEST_COMMANDLINE -eq 1 ]
then
    echo "make pytest"
    make pytest
else
    # As compared to ML pytest:
    #       No coverage
    #       Stop at first error
    echo "poetry run pytest -xsvv -n 4 --randomly-dont-reorganize --randomly-dont-reset-seed"
    poetry run pytest -xsvv -n 4 --randomly-dont-reorganize --randomly-dont-reset-seed

fi