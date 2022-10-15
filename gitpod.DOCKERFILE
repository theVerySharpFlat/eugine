FROM gitpod/workspace-full-vnc
RUN sudo apt-get -y update
RUN sudo apt-get -y upgrade
RUN sudo apt install -y xorg-dev 
RUN sudo apt install -y libglfw3-dev
RUN wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
RUN sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.3.224-focal.list https://packages.lunarg.com/vulkan/1.3.224/lunarg-vulkan-1.3.224-focal.list
RUN sudo apt install -y vulkan-sdk
RUN sudo rm -rf /var/lib/apt/lists/*

# Install emscripten toolchain
# RUN cd ~ \
#     && git clone https://github.com/emscripten-core/emsdk.git \
#     && cd emsdk \
#     && git pull \
#     && ./emsdk install latest \
#     && ./emsdk activate latest
