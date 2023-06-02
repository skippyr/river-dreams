<h1>River Dreams</h1>
	<h2>Starting Point</h2>
		<p>River Dreams is a ZSH theme written in Rust that will bring up a new aesthetics inspired by tribal patterns to the ZSH shell. By providing a curated set of components that show information about your system and also containing a fallback text feature, it becomes a really good addition to the set of tools of nerds out there.</p>
		<p>Right below, there are some previews that you can check out.</p>
		<img src="./images/preview.gif"/>
		<img src="./images/preview_fallback.gif"/>
		<p>In both previews, River Dreams was used on the <a href="https://github.com/kovidgoyal/kitty">Kitty</a> terminal emulator, with the <a href="https://github.com/skippyr/flamerial">Flamerial</a> theme and <a href="https://github.com/be5invis/Iosevka">Iosevka</a> font (stylistic set <code>ss08</code>). The program used to list directory entries is <a href="https://github.com/skippyr/reveal">reveal</a>.</p>
	<h2>Features</h2>
		<p>In its left prompt you will find:</p>
		<ul>
			<li>Your host name and local IPV4 address.</li>
			<li>The usage percentage of your disk.</li>
			<li>A calendar that shows week day, month and day.</li>
			<li>A clock that shows the hour and minute in 24 hours format.</li>
			<li>A decorator to show if you are root or not.</li>
			<li>A decorator if your last command failed.</li>
			<li>The name of sourced virtual environments.</li>
			<li>Your current directory path abbreviated.</li>
			<li>The name of the branch if you are inside a <code>git</code> repository.</li>
			<li>A lock decorator if you are not the user owner of the current directory.</li>
		</ul>
		<p>In its right prompt, you will find:</p>
		<ul>
			<li>The quantity of executable files, hidden entries, symlinks and broken entries (like dangling symlinks) in your current directory.</li>
			<li>The quantity of jobs running in the background for the current shell session.</li>
		</ul>
	<h2>Installation And Usage</h2>
		<p>River Dreams works on top of the ZSH shell, that you can find in any UNIX-like operating system like MacOS or Linux. If you are using Windows, you can take a look into setting it up on WSL (Windows Subsystem for Linux) or a virtual machine.</p>
		<p>First, install all the required dependencies to build and run it:</p>
		<ul>
			<li>The ZSH shell:</li>
				<p>This is the shell this theme applies on.</p>
			<li>The git version control software:</li>
				<p>This tool will be used to clone this project easily.</p>
			<li>The Rust Toolchain:</li>
				<p>This is the bundle with all the tools required to compile and run projects using the Rust programming language.</p>
				<p>You will find all the information you need to install the latest release from their <a href="https://www.rust-lang.org/">official website</a>.</p>
			<li>The Noto Sans font and font patched by the Nerd Fonts project.</li>
				<p>These fonts will provide you with all the cool symbols used in the theme. They are optional, as you can run River Dreams using its fallback feature, however they can improve your experience.</p>
				<p>The Noto Sans font can be downloaded from the <a href="https://fonts.google.com/noto/specimen/Noto+Sans">Google Fonts website</a>.</p>
				<p>You can download one font from the Nerd Font project from their <a href="https://github.com/ryanoasis/nerd-fonts/releases">Releases page</a> on GitHub.</p>
		</ul>
		<p>After installing the dependencies, choose one of the following ways to install River Dreams:</p>
		<h3>Stand Alone Installation (recommended)</h3>
			<p>By following this installation, you will be able to install River Dreams by directly running its entry point script, so you do not need to install any other dependency.</p>
			<ul>
				<li>Use git to clone this repository to a directory in your machine.</li>
					<pre><code>git clone --depth=1 https://github.com/skippyr/river_dreams ~/.config/zsh/themes/river_dreams</code></pre>
				<li>Add a source rule in the file <code>~/.zshrc</code> to include the theme file <code>river_dreams.zsh-theme</code> that is inside the repository cloned.</li>
					<pre><code>source ~/.config/zsh/themes/river_dreams/river_dreams.zsh-theme</code></pre>
				<li>Restart your ZSH session.</li>
				<li>Wait for the code to compile and the theme to start running.</li>
			</ul>
			To uninstall it:
			<ul>
				<li>Remove the repository cloned.</li>
					<pre><code>rm -rf ~/.config/zsh/themes/river_dreams</code></pre>
				<li>Remove the source rule used in the file <code>~/.zshrc</code>.</li>
			</ul>
		<h3>Installation Within OhMyZSH</h3>
			<p>By following this installation, you will be able to install River Dreams by using the OhMyZSH framework.</p>
			<ul>
				<li>Install OhMyZSH.</li>
					<p>You can find more information about it in its <a href="https://github.com/ohmyzsh/ohmyzsh">page on GitHub</a>.</p>
				<li>Clone this repository to the OhMyZSH custom themes's directory.</li>
					<pre><code>git clone --depth=1 https://github.com/skippyr/river_dreams ${ZSH_CUSTOM:-${HOME}/.oh-my-zsh/custom}/themes/river_dreams</code></pre>
				<li>Change the value of the variable <code>ZSH_THEME</code> in the file <code>~/.zshrc</code> to use the River Dreams theme.</li>
					<pre><code>ZSH_THEME="river_dreams/river_dreams"</code></pre>
				<li>Restart your ZSH session.</li>
				<li>Wait for the code to compile and the theme to start running.</li>
			</ul>
			To uninstall it:
			<ul>
				<li>Remove the repository clone.</li>
					<pre><code>rm -rf ${ZSH_CUSTOM:-${HOME}/.oh-my-zsh/custom}/themes/river_dreams</code></pre>
				<li>Change the value of the variable <code>ZSH_THEME</code> in the file <code>~/.zshrc</code> to a valid OhMyZSH theme.</li>
					<pre><code>ZSH_THEME="robbyrussell"</code></pre>
			</ul>
	<h2>Customization</h2>
		<p>You can toggle the fallback feature of River Dreams by using the function <code>river_dreams::toggle_fallback_text</code>. If you need more control of it, you can use an environment variable called <code>RIVER_DREAMS_USE_FALLBACK_TEXT</code>. That variable expects an integer value that represents a boolean:</p>
		<ul>
			<li><code>0</code> means <code>false</code>, which will disable the feature.</li>
			<pre><code>export RIVER_DREAMS_USE_FALLBACK_TEXT="0"</code></pre>
			<li><code>1</code> means <code>true</code>, which will enable the feature.</li>
			<pre><code>export RIVER_DREAMS_USE_FALLBACK_TEXT="1"</code></pre>
		</ul>
		<p>If it is not set, River Dreams will automatically set it a value based on your terminal emulator's color support.</p>
		<p>Further more, River Dreams does not offers another user-friendly way of changing its look. If you need to tweak it, you will need to change its source code, then recompile it again. If that is your case, the shell script that you source to start it exposes a function to compile the code easily, which you can use:</p>
		<pre><code>river_dreams::compile</code></pre>
	<h2>See Also</h2>
			<p>Thanks to <a href="https://github.com/unixorn">unixorn</a>, River Dreams is now available in the <a href="https://github.com/unixorn/awesome-zsh-plugins">awesome-zsh-plugins</a> repository. It hosts a lot of other cool ZSH themes, plugins, frameworks and more. So what about going there to see what else you like?</p>
		<h2>Issues And Contributions</h2>
			<p>Learn how to report issues, questions and ideas and how to contribute to this project by reading its <a href="https://skippyr.github.io/materials/pages/contributions_guidelines.html">contributions guidelines</a>.</p>
		<h2>License</h2>
			<p>This project is released under terms of the MIT License. You can refer to the license as the file LICENSE in the repository's root directory.</p>
			<p>Copyright (c) 2023, Sherman Rofeman. MIT License.</p>

