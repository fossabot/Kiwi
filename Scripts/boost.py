#!/usr/bin/env python

##### IMPORT #################################################################

import os
import sys
import argparse
import subprocess
import platform


root_dir = os.path.split(os.path.dirname(os.path.realpath(__file__)))[0]
flip_dir = os.path.join(root_dir, "ThirdParty", "flip")
flip_commit = "c47e41da05"
flip_url = "http://developer.irisate.com.s3-website-us-east-1.amazonaws.com/files/"

#==============================================================================
# Name : get_private_commit
#==============================================================================

def get_private_commit():
    commit  = subprocess.check_output(["git", "submodule", "status", "flip"])
    return commit[1:11]

#==============================================================================
# Name : get_public_commit
#==============================================================================

def read_commit():

    commit = ""

    config_file = os.path.join(flip_dir, "config")

    if  os.path.exists(config_file):
        fo = open(config_file, "r")
        commit = fo.readline()
        fo.close()

    return commit[0:10]

#==============================================================================
# Name : set_public_commit
#==============================================================================

def write_commit(commit):

    config_file = os.path.join(flip_dir, "config")
    fo = open(config_file, "w")
    fo.write(commit)
    fo.close()


#==============================================================================
# Name : init_boost_macos
#==============================================================================

def init_boost_macos():

    subprocess.check_call("./bootstrap.sh toolset=clang macosx-version-min=10.8 architecture=combined link=static", shell= True)

    subprocess.check_call("./b2 headers", shell= True)

    subprocess.check_call("./b2 address-model=32_64 --with-system stage", shell= True)

#==============================================================================
# Name : download_flip_windows
#==============================================================================

def download_flip_windows(commit):

    archive_file = "flip-demo-windows-" + commit + ".zip"

    # download archive
    print "-- downloading flip archive"
    urllib.urlretrieve (flip_url + archive_file, os.path.join(root_dir, "ThirdParty", archive_file))

    # extract archive
    print "-- extracting archive"

    os.mkdir(flip_dir)
    archive = zipfile.ZipFile(os.path.join(root_dir, "ThirdParty", archive_file), 'r')
    archive.extractall(flip_dir)
    archive.close()

    os.remove(os.path.join(root_dir, "ThirdParty", archive_file))

#==============================================================================
# Name : download_flip_linux
#==============================================================================

def init_boost_linux():

    subprocess.check_call("./bootstrap.sh toolset=gcc link=static", shell= True);

    subprocess.check_call("./b2 headers", shell= True);

    subprocess.check_call("./b2 --with-system stage", shell= True);

#==============================================================================
# Name : init_boost
#==============================================================================

def init_boost():

    os.chdir(os.path.join(root_dir, "ThirdParty", "boost"));

    if (platform.system() == "Darwin"):
        init_boost_macos()
    elif (platform.system() == "Windows"):
    	init_boost_windows()
    elif platform.system() == "Linux":
    	init_boost_linux()

    os.chdir(root_dir);

#==============================================================================
# Name : main
#==============================================================================

print "-- Initializing boost"

init_boost();

print "-- Boost initialized"
