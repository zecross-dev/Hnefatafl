cmake_minimum_required(VERSION 3.10)
project(Hnefatafl)

# Spécifier le standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Inclure le dossier des headers
include_directories(Headers)

# Lister les fichiers sources (ajoutez vos autres .cpp ici)
file(GLOB_RECURSE SOURCES "Sources/*.cpp" "main.cpp")

# Créer l'exécutable
add_executable(Hnefatafl ${SOURCES})
