#!/bin/bash -e

mv Build/Release/KiwiBuild/Bin/Release/Kiwi.app Kiwi.app
zip -r Kiwi.zip Kiwi.app