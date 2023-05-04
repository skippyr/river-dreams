<h1>River Dreams</h1>
	<h2>Starting Point</h2>
	<p>A multi line theme for ZSH.</p>
	<img src="./images/default_preview.png"/>
	<p>In the image, River Dreams was used on the <a href="https://github.com/kovidgoyal/kitty">Kitty</a> terminal emulator with the <a href="https://github.com/skippyr/flamerial">Flamerial</a> theme and <code>FiraMono Nerd Font</code> font.</p>
	<p>Below, there are descriptions of what the prompt has to offer:</p>
	<p>In the left prompt you will find:</p>
	<ul>
		<li>A separator to isolate the prompt from the output of the last command.</li>
		<li>Your host name and local ipv4 address.</li>
		<li>The in-use percentage of your main disk.</li>
		<li>A calendar that shows the week day, month, day and year.</li>
		<li>A clock that shows the hour and minute.</li>
		<li>The exit code of your last command if it has failed.</li>
		<li>Your user name.</li>
		<li>The path of sourced virtual environment abbreviated if you are using <code>virtualenv</code>.</li>
		<li>Your current directory path abbreviated.</li>
		<li>The name of the branch if you are inside of a directory tracked by <code>git</code>.</li>
	</ul>
	<p>In the right prompt you will find:</p>
	<ul>
		<li>The quantity of executables, hidden files and symbolic links in your current directory.</li>
		<li>The quantity of jobs running in the background for the current shell session.</li>
	</ul>
	<p>River Dreams also contains a fallback text feature, that allows you to use it even when not using a modern terminal emulator.</p>
	<img src="./images/preview_with_fallback.png"/>
	<h2>Installation And Usage</h2>
		<ul>
			<li>Install <a href="https://www.rust-lang.org/">Rust development tools</a>.</li>
			<p>These are the compiler and tools needed to compile River Dreams's source code.</p>
			<li>Install a font patched by the <a href="https://github.com/ryanoasis/nerd-fonts">Nerd Fonts</a> project and the <a href="https://fonts.google.com/noto/specimen/Noto+Sans">Noto Sans Font Family</a>.</li>
			<p>These fonts contains pretty symbols that are used in the prompt. It is highly recommend to use one to improve your experience, however, they are optional as you can run River Dreams with its fallback text feature.</p>
			<li>Install a great terminal emulator, like <a href="https://github.com/kovidgoyal/kitty">Kitty</a>.</li>
			<p>Even that River Dreams and ZSH can run basically in any terminal emulator, using one that is fast and that can handle unicode character really well will improve your experience.</p>
			<li>Download this repository to a directory in your machine.</li>
			<p>If you have <code>git</code> installed, you can download it in the following command:</p>
			<pre><code>git clone --depth 1 https://github.com/skippyr/river_dreams ~/.config/zsh/themes/river_dreams</code></pre>
			<p>This command will clone this repository to the directory <code>~/.config/zsh/themes/river_dreams</code>, however fell free to change it in the example. Just remember the path you have used because you will need it for the next step. The flag <code>--depth</code> with value <code>1</code> specifies to <code>git</code> that you only want to download the last commit, instead of the whole commit tree as it does by default.</p>
			<p>If you do not have <code>git</code> installed, do not worry, as you can also download the repository from GitHub. Just access the page of the project there, click on the <code>Code</code> button on the top of the page, then on <code>Download ZIP</code>. This will download a ZIP file, you just have to unzip it in the directory you want it to be.</p>
			<li>Add a source rule in your ZSH configuration file: <code>~/.zshrc</code> to source the theme file <code>river_dreams.zsh-theme</code> that is in the root directory of the repository that you have just downloaded.</li>
			<pre><code>source ~/.config/zsh/themes/river_dreams/river_dreams.zsh-theme</code></pre>
			<p>If you have chosen other directory in the last step, just change it in the source command. Pay attention to use <code>/river_dreams.zsh-theme</code> in the end of the path to include the theme file instead of the directory itself.</p>
			<li>Restart your ZSH session.</li>
			<p>At this point, the source code will be automatically compiled and, when it finishes, the prompt will be ready for you to use.</p>
		</ul>
	<h2>Customization</h2>
		<p>River Dreams can be customized through the use of environment variables.</p>
		<ul>
			<li><code>RIVER_DREAMS_USE_FALLBACK_TEXT</code></li>
			<p>This environment variable allow you to activate/deactivate the use of fallback text. This can be useful when you can not have access to a terminal font with pretty symbols:</p>
			<p>Activate it by exporting it with value <code>1</code>.</p>
			<pre><code>export RIVER_DREAMS_USE_FALLBACK_TEXT="1"</code></pre>
			<p>Deactivate it by exporting it with any other value like <code>0</code> for example.</p>
			<pre><code>export RIVER_DREAMS_USE_FALLBACK_TEXT="0"</code></pre>
			<p>If you do not set it, River Dreams will automatically set it for your current shell session based in your terminal's color support: if your terminal only supports the 3-bits color palette (ansi values from 0 to 8), it will activate it by setting it with value <code>1</code>. If not, it will deactivate it with value <code>0</code>. I know that it sounds weird, but that is the only effective method that I have found to determinate if you are possibling using an old terminal or not.</p>
			<p>This variable can make changes even when River Dreams is already running.</p>
		</ul>
		<p>Further more, if you know Rust, you can change River Dreams's source code and make it be however you would like it to be. After making your changes, you can recompile the source again in your current ZSH session by using the function <code>river_dreams::compile</code></p>
		<pre><code>river_dreams::compile</code></pre>
	<h2>Know issues</h2>
		<p>During tests, I could not make River Dreams work perfectly with other popular plugins for ZSH such as <code>zsh-autosuggestions</code> and <code>zsh-syntax-highlighting</code>: when the terminal gets cleared, the separator at its top disappears. I believe that this issue is due to it be a multiline prompt, as I also had issues when using <code>zle</code> to reset this type of prompt, and it was kinda tricky to make it to work.</p>
	<h2>See Also</h2>
		<p>Thanks to <a href="https://github.com/unixorn">unixorn</a>, River Dreams is now available in the <a href="https://github.com/unixorn/awesome-zsh-plugins">awesome-zsh-plugins</a> repository. It hosts a lot of other cool ZSH themes, plugins, frameworks and more. So what about going there to see what else you like?</p>
	<h2>Issues And Contributions</h2>
		<p>Learn how to report issues, questions and ideas and how to contribute to this project by reading its <a href="https://skippyr.github.io/materials/pages/contributions_guideline.html">contributions guideline</a>.</p>
	<h2>License</h2>
		<p>River Dreams is released under the MIT License. You can refer to the license as the file <code><a href="https://github.com/skippyr/river_dreams/blob/main/LICENSE">LICENSE</a></code> in the root directory of this repository.</p>
		<p>Copyright (c) 2023, Sherman Rofeman. MIT License.</p>
