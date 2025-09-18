<h1> MyWallet (<a href = "https://saucer.app/" target="_blank" rel="noopener noreferrer" >with SAUCER</a>)</h1>

A cross-platform, lightweight finance management desktop application that blends:
- SvelteKit, TypeScript, TailwindCSS and DaisyUI for the Frontend
- C++ backend that persists data using a local SQLite database.

<img width="1920" height="1080" alt="Screenshot From 2025-09-18 03-54-28" src="https://github.com/user-attachments/assets/5a109c4b-d63d-4978-8ac7-cb9ddfff9880" />

## 📖 What Does It Do?

**MyWallet** allows you to easily monitor your capital flow.  

- You can create **wallets** that represent your different resources.  
  - Each wallet has a **name**, **currency**, **type**, **initial amount**, **current balance**, and even a customizable **color** for aesthetics.  

- Within each wallet, you can record both **incomes** and **expenses**.  
  - Each transaction includes a **category**, a **type** (income or expense), an **amount**, and an optional **description**.  

- Starting with two wallets, you can perform **transfers** between them.  
  - ⚠️ Both wallets must use the **same currency** for a transfer to work.  

- You can **archive** wallets you no longer need.  
  - Archived wallets are moved to the **Recycle Bin**, where you can choose to either **restore** them or **permanently delete** them.  
  - Deleting a wallet also deletes **all associated transactions**.  

- Both wallets and individual transactions can be **edited** or **deleted** at any time.  

- The app features an **offline login system** to enforce security.  
  - You can **log out** after finishing your session.  
  - You can also change your **profile picture**.  
  - ⚠️ If you forget your password after logging out, you will need to **reset the application**, which erases all your data. To avoid this, remember your password carefully — or simply never log out.  

- A built-in **theme switcher** lets you personalize the app’s appearance to match your preferences.  


## 🚀 Get Started

### 🐧 Fedora (Tested on Fedora 42 — Fresh Install)
```bash
# 1. Install dependencies
sudo dnf install pnpm gcc gcc-c++ make cmake gtk4-devel libadwaita-devel webkitgtk6.0-devel sqlite-devel

# 2. Clone the repository
git clone https://github.com/EnamAntoineM/MyWallet.git
cd MyWallet/

# 3. Build the project
pnpm install
pnpm run build
cmake -B build -S .
cmake --build build --config Release
cpack --config build/CPackConfig.cmake

# 4. Run the app
# The executable is located in the build/ folder. Double-click on it to open.
# OR
cd build
./myWallet
```
---

### 🐧 Ubuntu 24.04 LTS (Noble Numbat — Tested)
```bash
# 1. Install GCC 13 & GCC 14 side by side
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt update
sudo apt install -y curl git build-essential cmake \
    libgtk-4-dev libadwaita-1-dev libwebkitgtk-6.0-dev libsqlite3-dev \
    gcc-13 g++-13 gcc-14 g++-14

# 2. Register both with update-alternatives
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 130
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 140
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-13 130
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-14 140

# 3. Install Node.js 22 + pnpm (via nvm + corepack)
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.40.3/install.sh | bash
. "$HOME/.nvm/nvm.sh"
nvm install 22
corepack enable
corepack prepare pnpm@latest --activate

# 4. Switch to GCC 14 for MyWallet build
sudo update-alternatives --set gcc /usr/bin/gcc-14
sudo update-alternatives --set g++ /usr/bin/g++-14

# 5. Clone and build MyWallet
git clone https://github.com/EnamAntoineM/MyWallet.git
cd MyWallet/
pnpm install
pnpm run build
cmake -B build -S .
cmake --build build --config Release
cpack --config build/CPackConfig.cmake

# 6. Switch back to GCC 13 after the build
sudo update-alternatives --set gcc /usr/bin/gcc-13
sudo update-alternatives --set g++ /usr/bin/g++-13

# The executable is located in the build/ folder. Double-click on it to open.
```
---

### 🐧 Arch Linux (Tested)
```bash
# 1. Install system dependencies
sudo pacman -Syu --needed base-devel git cmake \
    gtk4 libadwaita webkitgtk-6.0 sqlite curl pnpm

# 2. Clone and build frontend
git clone https://github.com/EnamAntoineM/MyWallet.git
cd MyWallet/
pnpm install
pnpm run build

# 3. Build backend
cmake -B build -S .
cmake --build build --config Release
cpack --config build/CPackConfig.cmake

# 4. Run
# The executable is located in the build/ folder. Double-click on it to open.
# OR
cd build
./myWallet
```
---

### 🪟 Windows 10/11 (Tested)
```
1. Download the installer from the latest release:
   https://github.com/EnamAntoineM/MyWallet/releases/latest

2. Run the downloaded .exe installer  
   IMPORTANT: Choose "Desktop" as the installation location  
   Do NOT install into "C:\Program Files" or other protected directories.  

3. Launch the app from the Desktop icon or from the folder `MyWallet\` on the Desktop
```
### 🍎 macOS
```zsh
# 1. Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 2. Install dependencies
brew install cmake sqlite webkit2-gtk libadwaita gtk4 node pnpm

# 3. Clone and build MyWallet
git clone https://github.com/EnamAntoineM/MyWallet.git
cd MyWallet/
pnpm install
pnpm run build
cmake -B build -S .
cmake --build build --config Release
cpack --config build/CPackConfig.cmake

# 4. Run the app
# The executable is located in the build/ folder. Double-click on it to open.
# OR
cd build
./myWallet
```
---
---

## 👥 Contributors
[![Contributors](https://contrib.rocks/image?repo=EnamAntoineM/MyWallet)](https://github.com/EnamAntoineM/MyWallet/graphs/contributors)


<img width="1200" height="512" alt="Commits over time" src="https://github.com/user-attachments/assets/fac3df3c-b91c-49ef-b291-d0f6937e08c4" />
