##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=cube
ConfigurationName      :=Debug
WorkspacePath          :=/home/tchv/StartPP/StartPP
ProjectPath            :=/home/tchv/wxWidgets-3.1.0/build-linux-debug/samples/opengl/cube
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=tchv
Date                   :=27.11.2017
CodeLitePath           :=/home/tchv/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="cube.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  $(shell /usr/local/bin/wx-config --libs std,gl --static=no --debug=yes)
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)/usr/include $(IncludeSwitch)/usr/include/freetype2 $(IncludeSwitch)/usr/include/gtk-3.0 $(IncludeSwitch)/usr/lib/x86_64-linux-gnu/glib-2.0/include $(IncludeSwitch)/usr/include/glib-2.0 $(IncludeSwitch)/usr/include/pango-1.0 $(IncludeSwitch)/usr/include/cairo $(IncludeSwitch)/usr/include/gdk-pixbuf-2.0/ $(IncludeSwitch)/usr/include/atk-1.0 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)GL $(LibrarySwitch)X11 $(LibrarySwitch)gtk-3 $(LibrarySwitch)gdk-3 $(LibrarySwitch)cairo $(LibrarySwitch)gobject-2.0 
ArLibs                 :=  "GL" "X11" "gtk-3" "gdk-3" "cairo" "gobject-2.0" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -std=c++11 $(shell /usr/local/bin/wx-config --cxxflags --debug=yes --static=no) $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
GDK_GL:=legacy
Objects0=$(IntermediateDirectory)/cube.cpp$(ObjectSuffix) $(IntermediateDirectory)/glcanvas.cpp$(ObjectSuffix) $(IntermediateDirectory)/glx11.cpp$(ObjectSuffix) $(IntermediateDirectory)/GDKCanvas.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/cube.cpp$(ObjectSuffix): cube.cpp $(IntermediateDirectory)/cube.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/tchv/wxWidgets-3.1.0/build-linux-debug/samples/opengl/cube/cube.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cube.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cube.cpp$(DependSuffix): cube.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cube.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/cube.cpp$(DependSuffix) -MM cube.cpp

$(IntermediateDirectory)/cube.cpp$(PreprocessSuffix): cube.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cube.cpp$(PreprocessSuffix) cube.cpp

$(IntermediateDirectory)/glcanvas.cpp$(ObjectSuffix): glcanvas.cpp $(IntermediateDirectory)/glcanvas.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/tchv/wxWidgets-3.1.0/build-linux-debug/samples/opengl/cube/glcanvas.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/glcanvas.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/glcanvas.cpp$(DependSuffix): glcanvas.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/glcanvas.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/glcanvas.cpp$(DependSuffix) -MM glcanvas.cpp

$(IntermediateDirectory)/glcanvas.cpp$(PreprocessSuffix): glcanvas.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/glcanvas.cpp$(PreprocessSuffix) glcanvas.cpp

$(IntermediateDirectory)/glx11.cpp$(ObjectSuffix): glx11.cpp $(IntermediateDirectory)/glx11.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/tchv/wxWidgets-3.1.0/build-linux-debug/samples/opengl/cube/glx11.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/glx11.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/glx11.cpp$(DependSuffix): glx11.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/glx11.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/glx11.cpp$(DependSuffix) -MM glx11.cpp

$(IntermediateDirectory)/glx11.cpp$(PreprocessSuffix): glx11.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/glx11.cpp$(PreprocessSuffix) glx11.cpp

$(IntermediateDirectory)/GDKCanvas.cpp$(ObjectSuffix): GDKCanvas.cpp $(IntermediateDirectory)/GDKCanvas.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/tchv/wxWidgets-3.1.0/build-linux-debug/samples/opengl/cube/GDKCanvas.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GDKCanvas.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GDKCanvas.cpp$(DependSuffix): GDKCanvas.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GDKCanvas.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GDKCanvas.cpp$(DependSuffix) -MM GDKCanvas.cpp

$(IntermediateDirectory)/GDKCanvas.cpp$(PreprocessSuffix): GDKCanvas.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GDKCanvas.cpp$(PreprocessSuffix) GDKCanvas.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


