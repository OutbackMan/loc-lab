# Loc Lab Contributing

## Required Downloads
|                    | Desktop                                                                           | Android             | iOS        |
| ------------------ | --------------------------------------------------------------------------------- | ------------------- | ---------- |
| Windows            | [cmake][1], [llvm 8+][2], [mingw-w64](https://sourceforge.net/projects/mingw-w64) | [android studio][3] | N/A        |
| Ubuntu 18.04[^1] | [cmake][1], [llvm 8+][2]                                                          | [android studio][3] | N/A        |
| macOS              | [cmake][1], [llvm 8+][2], [xcode command line tools][4]                           | [android studio][3] | [xcode][4] |

[^1]: In reality, ll has the potential to support many other gnu/linux distributions. Ubuntu is specified as it provides a somewhat standardized environment.
[1]: https://cmake.org/download
[2]: http://releases.llvm.org/
[3]: https://developer.android.com/studio
[4]: https://developer.apple.com/downloads/more

## Optional Downloads
Not necessary for development, however I would like to mention these cross platform programs that I use for desktop development:
* Text Editor: [gvim](https://www.vim.org/download.php) / [vim-gtk3](https://packages.ubuntu.com/xenial/vim-gtk3) / [MacVim](https://github.com/macvim-dev/macvim) 
* Debugger: [jre](https://www.oracle.com/technetwork/java/javase/downloads/jre8-downloads-2133155.html) + [cdt stand-alone debugger](https://www.eclipse.org/cdt/downloads.php)
Also, to enforce non-repudiation of git tags [gpg](https://gnupg.org/download/)

## Compiling
### Desktop
1. Build SDL2. Simply run the build script appropriate for your OS inside the 'sdl2-2.0.9-minimal' folder, e.g. on windows `windows-build.bat`.
2. Build ll. Follow the same procedure as for SDL2, except this time in the root ll folder.
'INFO' comments are left in these build scripts indicating various changes that can be made to alter compilation output. If using these changes, I advise you to replace comments with what you have changed.
### Mobile
For android, open the existing 'android-project' folder inside android studio and build. 
Similarly for ios, open the existing 'ios-project' folder inside xcode and build. 

## Git Workflow 
Work for a feature will start on new branch.
```bash
# after forking on Github...
git clone https://github.com/<username>/ll.git
cd ll
git remote add upstream https://github.com/OutbackMan/ll.git

# Start working and saving to your fork.
git checkout -b my-feature-branch
git push origin my-feature-branch 

# Rebase regularly with upstream branch to avoid having to 3-way-merge.
git pull --rebase=interactive upstream upstream/<starting-branch>
# If something goes amiss:
git reflog; git reset --hard HEAD@{2}

# Once feature is ready, push and submit a pull request on your fork.
git push origin my-feature-branch
```

As git supports interoperability between VCSs, it is very easy to impersonate a user by simple altering username and email. As such, I digitally sign all tags with GPG.[^2]
The relevent GPG keys to import are listed in the [AUTHORS file](AUTHORS.md).
[^2]: Due to the DAG structure of commits in git only one commit has to be signed to effectively verify parent commits.
A brief overview of GPG usage:
```bash
# Generate key
gpg --full-generate-key
# Record fingerprint of public key (it is the 40 character hex string)
gpg --list-keys

# Upload public key information to key server
gpg --keyserver pgp.mit.edu --send-keys <fingerprint>
# Retrieve public key information from key server
gpg --keyserver pgp.mit.edu --recv-keys <fingerprint>

# Export your private key in ascii format to move to another computer
gpg --export-secret-keys --armor <fingerprint> > my-private-key.asc
# Import your private key
gpg --import my-private-key.asc

# Setup git to use your private key 
git config --global user.signingkey <fingerprint>
# Sign git tag
git tag -s <tag> -m 'signed tag'
# Verify git tag
git verify-tag <tag>
```

## LL Architecture
I utilize a unity build as it drastically simplifies the build script and reduces I/O and link times.
Furthermore, I don't use any build tools as they add a layer of complexity I deem unessecary for this project.
All source files go in the `code/` folder. 
Any source files who implement functionality supported by the C standard library (except I/O) and/or gcc/clang intrinsics are given a standard name, e.g. `defs.h`.
Any source files who implement functionality dependent on the platform layer are prefixed with the name 'll', e.g. `ll-entity.c`.
Any source files who implement functionality for a platform layer are prefixed with the name of the platform, e.g. `sdl-ll.c`.

> NOTE: As unity build, build scripts don't have to be modified to include new source files. Simply include them in the source file of the appropriate entry point.
