#!/bin/bash

# ✅ ❌

var_start=""
var_end=""
dir_input="input"
dir_build="build"
dir_exe="exe"
dir_log="log"
dir_externals="external"

SCRIPT="./run.sh"
LOG="../output/start.log"

CALLING_ARGUMENT="$1"

dir_run_time_config="run_time_config"
path_DONE_installed="${dir_run_time_config}/DONE_installed.txt"

silent_come_back() { cd - > /dev/null; }

clear_file() { if [ -f $1 ]; then rm -rf $1; fi; }
clear_file_nested() { cd $1; clear_file "$2"; silent_come_back; }

clear_dir() { if [ -d $1 ]; then rm -rf $1; fi; mkdir $1; }
clear_dir_nested() { cd $1; clear_dir "$2"; silent_come_back; }

create_dir() { if [ ! -d $1 ]; then mkdir $1; fi; }
create_dir_nested() { cd $1; create_dir "$2"; silent_come_back; }

install_hook()
{
    DIR_SCRIPTS="scripts"
    PATH_MY_HOOK="$DIR_SCRIPTS/formating_hook.sh"
    PATH_GIT_HOOK=".git/hooks/pre-commit"

    cp $PATH_MY_HOOK $PATH_GIT_HOOK && chmod +x $PATH_GIT_HOOK
    chmod +x $PATH_GIT_HOOK
}

timer_start() { var_start=$(date +%s); }
timer_end() { var_end=$(date +%s); }
timer_print()
{
    elapsed=$((var_end - var_start))
    hours=$((elapsed / 3600))
    minutes=$(( (elapsed % 3600) / 60 ))
    seconds=$((elapsed % 60))
    printf "\nProgram          - took: %02d:%02d:%02d\n" $hours $minutes $seconds
}
install_packages()
{
    if [ -f $path_DONE_installed ]; then return; fi

    # Funkcja sprawdzająca czy pakiet jest zainstalowany
    check_and_install()
    {
        PACKAGE=$1
        if ! dpkg-query -W -f='${Status}' "$PACKAGE" 2>/dev/null | grep "install ok installed" > /dev/null; then
            echo "$PACKAGE is not installed. Installing..."

            sudo apt install -y "$PACKAGE"

            if [ $? -eq 0 ]; then
                echo ""
            else
                echo -e "\nstart_all.sh - ERROR - unable to install this package: $PACKAGE\n"
                exit
            fi
        fi
    }

    # Lista pakietów do zainstalowania
    PACKAGES=(
        tar
        make
        cmake
        build-essential
        gcc
        g++
        libstdc++-11-dev
        gcc-multilib
        g++-multilib
    )

    # Aktualizacja listy pakietów
    sudo apt update -y > /dev/null 2>&1 && sudo apt upgrade -y > /dev/null 2>&1

    # Sprawdzanie i instalowanie każdego pakietu
    for PACKAGE in "${PACKAGES[@]}"; do
        check_and_install "$PACKAGE"
    done


    echo -ne "\n\n"
    echo "Instalation completed"
    echo "DONE" > $path_DONE_installed
    echo -ne "\n\n"
}
env_prep()
{
    create_dir "$dir_input"
    create_dir "$dir_build"
    create_dir "$dir_exe"
    create_dir "$dir_log"
    create_dir "$dir_run_time_config"

    # ONLY one is active at a time #  erase '-' to be able to to ex. -ltc

    if [[ "$CALLING_ARGUMENT" == *"-c"* ]]; then
    {
        clear_dir "$dir_build"
    }
    fi

    if [[ "$CALLING_ARGUMENT" == *"-t"* ]]; then
    {
        clear_dir "$dir_build"
        export FLAG_TESTING_ACTIVE="Yes"
    }
    fi

    if [[ "$CALLING_ARGUMENT" == *"-l"* ]]; then
    {
        clear_dir "$dir_build"
        export FLAG_BUILDING_LIBRARY="Yes"
    }
    fi

    chmod +x scripts/*.sh
}
create_my_libraries()
{
    git submodule update --remote
    git submodule update --init --recursive

    LIBS=(
        CORE
    )

    check_if_library_is_present_make_it_if_its_not()
    {
        LIB_NAME="$1"

        PATH_ROOT_DIR="${dir_externals}/${LIB_NAME}_lib"
        PATH_LIB="${PATH_ROOT_DIR}/build/lib${LIB_NAME}"

        if [[ ! (-f "${PATH_LIB}.a" || -f "${PATH_LIB}.so") ]]; then
        {
            cd $PATH_ROOT_DIR
            {
                ./start.sh -l
            }
            silent_come_back

            if [[ ! (-f "${PATH_LIB}.a" || -f "${PATH_LIB}.so") ]]; then
            {
                echo -e "\n\n\n\nUnable to compile lib -> ${LIB_NAME}";
                exit
            }
            fi
        }
        fi
    }

    for LIB in "${LIBS[@]}"; do
        check_if_library_is_present_make_it_if_its_not "$LIB"
    done
}

#####################   START   #####################

install_hook

env_prep

install_packages

create_my_libraries

timer_start
{
    cd scripts
    script -q -c "$SCRIPT 2>&1" /dev/null | tee $LOG
}
timer_end

timer_print
