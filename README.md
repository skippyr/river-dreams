<p align="center">
  <img alt="" src="imgs/ornament.webp" />
</p>
<h1 align="center">≥v≥v&ensp;riverdreams&ensp;≥v≥v</h1>
<p align="center">
  <img alt="" src="https://img.shields.io/github/license/skippyr/riverdreams?style=plastic&label=%E2%89%A5%20license&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/v/tag/skippyr/riverdreams?style=plastic&label=%E2%89%A5%20tag&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/commit-activity/t/skippyr/riverdreams?style=plastic&label=%E2%89%A5%20commits&labelColor=%2324130e&color=%23b8150d" />
  &nbsp;
  <img alt="" src="https://img.shields.io/github/stars/skippyr/riverdreams?style=plastic&label=%E2%89%A5%20stars&labelColor=%2324130e&color=%23b8150d" />
</p>
<h2>❡ About</h2>
<p>A tribal inspired zsh theme for Linux designed to help you develop your dream projects.</p>
<p align="center">
  <img alt="" src="imgs/preview.webp" />
  <p align="center"><sup><strong>Caption:</strong> a preview showing riverdreams applied on zsh in its own repository. The terminal theme used is <a href="https://github.com/skippyr/flamerial">flamerial</a> and font is <a href="https://fonts.google.com/specimen/Inconsolata">inconsolata</a>.</sup></p>
</p>
<p>Its left prompt displays:</p>
<ul>
  <li>Your local IPV4 address.</li>
  <li>Your disk use percentage.</li>
  <li>Your battery status and remaining charge, if you have one.</li>
  <li>A calendar showing the day of the week, month and day.</li>
  <li>A clock showing the hour and minute.</li>
  <li>A decorator when you are the root user.</li>
  <li>Different decorators for success and failure exit codes.</li>
  <li>Your active directory path, shorting it when you get inside of a git repository.</li>
  <li>The active branch, when you are inside of a git repository.</li>
  <li>A decorator when you do not have write permissions in the active directory.</li>
</ul>
<p>Its right prompt displays:</p>
<ul>
  <li>The total of each entry type in your active directory: files, directories, blocks, characters, fifos, sockets and symlinks.</li>
  <li>The total of jobs set on the background.</li>
</ul>
<h2>❡ Install</h2>
<h3>Dependencies</h3>
<p>In order to install it, the following dependencies must be installed:</p>
<ul>
  <li><strong>git</strong>: it will be used to clone this repository.</li>
  <li><strong>make, gcc</strong>: they will be used to compile this project.</li>
  <li><strong>Nerd Fonts Symbols, Noto Sans</strong>: these fonts provide the pretty symbols used by the theme.</li>
</ul>
<h3>Procedures</h3>
<h4>Manual Procedures</h4>
<p>If you want to install it without the use of a plugin manager or for a specific framework, follow these steps:</p>
<ul>
  <li>Clone its repository using <code>git</code>:</li>

```sh
git clone --depth 1 https://github.com/skippyr/riverdreams
                    ~/.config/zsh/themes/riverdreams;
```

  <li>Source its file <code>riverdreams.zsh-theme</code> in your <code>~/.zshrc</code> configuration file:</li>

```sh
source ~/.config/zsh/themes/riverdreams/riverdreams.zsh-theme;
```

  <li>Reopen <code>zsh</code>.</li>
</ul>
<h4>Procedures For OhMyZSH</h4>
<p>If you want to install and use it with the OhMyZSH framework, follow these steps:
<ul>
  <li>Clone its repository using <code>git</code>:</li>

```sh
git clone --depth 1 https://github.com/skippyr/riverdreams
                    ${ZSH_CUSTOM:-$HOME/.oh-my-zsh/custom}/themes/riverdreams;
```

  <li>Apply the theme in your <code>~/.zshrc</code> configuration file:</li>

```sh
ZSH_THEME="riverdreams/riverdreams";
```

  <li>Reopen <code>zsh</code>.</li>
</ul>
<h4>Procedures For Antidote</h4>
<p>If you want to install it using the <code>antidote</code> plugin manager, follow these steps:</p>
<ul>
  <li>Add its repository to your plugins file, usually <code>~/.zsh_plugins.txt</code>.</li>

```sh
skippyr/riverdreams
```

  <li>Use <code>antidote</code> to load your plugins:</li>

```sh
antidote load ~/.zsh_plugins.txt;
```

</ul>
<h2>❡ Help</h2>
<p>If you need help related to this project, open a new issue in its <a href="https://github.com/skippyr/riverdreams/issues">issues page</a> or send me an <a href="mailto:skippyr.developer@gmail.com">e-mail</a> describing what is going on.</p>
<h2>❡ Contributing</h2>
<p>This project is open to review and possibly accept contributions, specially fixes and suggestions. If you are interested, send your contribution to its <a href="https://github.com/skippyr/riverdreams/pulls">pull requests page</a> or to my <a href="mailto:skippyr.developer@gmail.com">e-mail</a>.</p>
<a>By contributing to this project, you must agree to license your work under the same license that the project uses.</a>
<h2>❡ License</h2>
<p>This project is licensed under the MIT License. Refer to the <code>LICENSE</code> file that comes in its source code for license and copyright details.</p>
&ensp;
<p align="center"><sup><strong>≥v≥v&ensp;Here Are Dragons!&ensp;≥v≥</strong><br />Made with love by skippyr <3</sup></p>