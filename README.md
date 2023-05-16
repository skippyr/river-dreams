<h1>River Dreams</h1>
	<h2>Starting Point</h2>
		<p>A tribal inspired theme for the ZSH shell that includes a fallback text feature.</p>
		<p>When dealing with shells, a theme for it means the prompt that you see when typing commands and not necessary the colors.</p>
		<p>River Dreams takes minimal aesthetics and features into account to bring you a graceful experience when using a terminal emulator.</p>
		<p>In the preview, below, you can see a demo of River Dreams with its default look.</p>
		<img src="./images/preview.gif"/>
		<p>For those moments where you can not have access to a rich terminal and have to use bitmap fonts, River Dreams will still be there with you: it includes a fallback text feature that is automatically activated based in your terminal emulator's color support. Or you can manually activate it too.</p>
		<img src="./images/preview_fallback.gif"/>
	<h2>Aesthetics</h2>
		<p>River Dreams is heavily inspired by tribal patterns, which you will see throughout the whole set of symbols I have chosen for it.</p>
		<p>It is also inspired in the look of other popular ZSH and Fish themes:</p>
		<ul>
			<li><code>robbyrussel</code> from the <a href="https://github.com/ohmyzsh/ohmyzsh">OhMyZSH</a> framework.</li>
			<li><a href="https://github.com/romkatv/powerlevel10k">Powerlevel10k</a>.</li>
			<li><a href="https://github.com/spaceship-prompt/spaceship-prompt">Spaceship Prompt</a>.</li>
			<li>The default theme for the <a href="https://fishshell.com/">Fish shell</a>.</li>
		</ul>
		<p>So, you will probably find some similarities with those themes too.</p>
	<h2>Features</h2>
		<p>A ZSH theme can be divided in two parts: a prompt that will be on the left side of your screen: the left prompt, and another that will be in the right side: the right prompt. River Dreams works with the same architecture and that is what will be used as reference now.</p>
		<p>In its left prompt you will find:</p>
		<ul>
			<li>A separator that fits your terminal emulator's width to isolate the prompt from the output of your last commands.</li>
			<li>Your local IPV4 address.</li>
			<li>The in-use percentage of the disk partition mounted at <code>/</code>.</li>
			<li>A calendar that shows the name of the month abbreviated and the day.</li>
			<li>A clock that shows the hour and minute in 24 hours format.</li>
			<li>A hash decorator to show if you are root or not.</li>
			<li>The name of sourced virtual environments.</li>
			<li>Your current directory path abbreviated, shrinking the path of home and <code>git</code> repositories directories.</li>
			<li>A lock decorator if you are not the user owner of the current directory.</li>
			<li>The name of the branch if you are inside a <code>git</code> repository.</li>
		</ul>
		<p>In its right prompt, you will find:</p>
			<ul>
				<li>The quantity of executable files, hidden files and symbolic links in your current directory.</li>
				<li>The quantity of jobs running in the background for the current shell session.</li>
			</ul>
	<h2>Speed</h2>
		<p>I knew that you would ask. River Dreams is built using Rust, a modern compiled programming language, that will take the best effort to make it fast for your daily usage.</p>
	<h2>Installation And Usage</h2>
		<p>Want to try out River Dreams? Awesome. In this section, let's talk about how to configure ZSH for your terminal emulator and how to install the River Dreams theme on top of it.</p>
		<p>Installation instructions are intended for Linux and MacOS operating systems, but if you are using Windows, you can still follow along by running a Linux virtual machine inside of WSL (Windows Subsystem for Linux).</p>
		<h3>Configuring the ZSH shell</h3>
			<p>A shell is the program that has the function to receive and interpret the commands that you type, and pass them to the system's kernel to execute.</p>
			<p>There are many shells applications, being ZSH one of them. One of its key features is that it is easier to customize, while maintaining compatibility with the syntax of the Bash shell.</p>
			<p>In MacOS, the ZSH shell is the default one since Catalina release. However, on Linux, Bash is the default one. For that reason, in this section you will learn how to configure ZSH as the default shell for your current user if it is not already.</p>
			<p>First, you will need to install it. In Linux, you can refer to your distribution's package manager to do so. For example, if you are using Ubuntu or based distributions of it, you can use the following command to install ZSH (sudo priveleges are required to do it):</p>
			<pre><code>sudo apt install -y zsh</code></pre>
			<p>After installing ZSH, it is time to make it the default one.</p>
			<p>Your user has a default shell set in the system. To change it to use ZSH, you can use the following command (your password will be required to do it):</p>
			<pre><code>chsh -s /bin/zsh</code></pre>
			<p>Normally, this is enough to make your terminal emulators to use ZSH instead of Bash. You probably just have to restart them. However, there are some terminal emulators that will have a special settings to configure it inside them, which will overwrite your system's default: like Gnome Terminal and Konsole do.</p>
			<p>If that is your case, you can go to their settings, and search for a section where you can choose the command to run as the startup shell. Use <code>/bin/zsh</code> to initiate ZSH.</p>
			<p>If you are asking, you can always revert those changes back to Bash, if it was the default one by using basically the same command if you need:</p>
			<pre><code>chsh -s /bin/bash</code></pre>
		<h3>Installing River Dreams</h3>
			<p>Now that you have ZSH running as your shell, it is time to install River Dreams.</p>
			<p>First, we need to install some dependencies and see some recomendations for a better experience. If you are using MacOS, you can install these dependencies from their official websites (hyperlinks will be available in the text) or even using <a href="https://brew.sh">Homebrew</a>. On Linux, you can do it too, but you will also find some of them using your distribution's package manager.</p>
			<ul>
				<li>Install <code>gcc</code> and Rust.</li>
				<p>These are the tools to compile the source code of River Dreams.</p>
				<p>Even that <code>gcc</code> is not used to compile River Dreams directly, it is needed by Rust to do any compilation.</p>
				<p>You can find more information about how to install Rust in its <a href="https://www.rust-lang.org">official website</a>.</p>
				<p>About the <code>gcc</code>, for example, if you are using Ubuntu or a distribution based on it, you can download it using the following command (sudo priveleges are required to do it):</p>
				<pre><code>sudo apt install -y build-essential</code></pre>
				<li>Install the Noto Sans font family and a monospaced font patched by the Nerd Fonts project.</li>
				<p>These are the fonts that will give access to all the symbols shown in the prompt. As you can run River Dreams using its fallback text feature, installing them is kinda optional, but you will probably want to use them for a better experience.</p>
				<p>The Noto Sans font has a wide range of character for many languages, for that reason you will probably have it installed in your operating system already. If not, you can always download it from its page on the <a href="https://fonts.google.com/noto/specimen/Noto+Sans">Google Fonts website</a>.</p>
				<p>If you are using Ubuntu or a distribution based on it, you can download it using the following command (sudo priveleges are required to do it) if you do not have it installed:</p>
				<pre><code>sudo apt install -y fonts-noto</code></pre>
				<p>Fonts patched by Nerd Fonts are usually distributed in a widely range of Linux distribution's package managers. However, you can always download one from the <a href="https://github.com/ryanoasis/nerd-fonts/releases">Releases</a> page of the Nerd Fonts project on GitHub.</p>
				<li>Install <code>git</code>.</li>
				<p>This is an optional dependency, however installing <code>git</code> will ease further installation steps and updating this software. If you prefer not to do it, you will have to do those manually.</p>
				<p>If you are using Ubuntu or a distribution based on it, you can download it using the following command (sudo priveleges are required to do it):</p>
				<pre><code>sudo apt install -y git</code></pre>
				<li>Install a good terminal emulator.</li>
				<p>This is a completly optional dependency, but a fair recommendation: using a terminal that is fast and that can render unicode characters pretty well will give you a smoother experience. For MacOS and Linux, Kitty is a good choice. If you are using Windows, try using Windows Terminal, it will also allow you to easily use WSL and PowerShell.</p>
				<p>If you are using Ubuntu or a distribution based on it, you can download Kitty using the following command (sudo priveleges are required to do it):</p>
				<pre><code>sudo apt install -y kitty</code></pre>
				<p>However, you can find more information about how to install Kitty in its <a href="https://github.com/kovidgoyal/kitty">repository</a> on GitHub.</p>
				<p>Kitty uses your user's default shell, which is pretty convenient as in last section you have learned how to configure it.</p>
			</ul>
		<p>Now that you have installed all the dependencies, to build River Dreams, it is finally the time to compile and run it. As River Dreams is a stand-alone theme, it does not required any framework to work, but some people will probably like to use it within the OhMyZSH framework. To satisfact both, here are instructions to do such:</p>
			<h4>Stand-Alone Installation (recommended)</h4>
				<p>This is the recommended installation for most users, as you will not have to install any other dependency.</p>
				<ul>
					<li>Download this repository to a directory in your machine.</li>
					<p>If you installed <code>git</code>, you can download it with the following command:</p>
					<pre><code>git clone --depth=1 https://github.com/skippyr/river_dreams ${HOME}/.config/zsh/themes/river_dreams</code></pre>
					<p>This command will clone this repository to the directory <code>${HOME}/.config/zsh/themes/river_dreams</code>, however feel free to change it to whatever directory seems formidable for you when you use the command. Just remember its path you have because you will need it for the next step.</p>
					<p>The flag <code>--depth</code> with value <code>1</code> specifies to <code>git</code> that you only want to download the last commit, instead of the whole commit tree as it does by default.</p>
					<p>If you did not install <code>git</code>, you can download the repository from its page on GitHub. Just access the page of the project there, then click on the button labeled <code>Code</code> on the top of the page, then click on the button labeled <code>Download ZIP</code> that will be in the floating menu that appears. This will download a ZIP file, you just have to unzip it in the directory you want it to be.</p>
					<li>Add a source rule in your ZSH configuration file at <code>${HOME}/.zshrc</code> to source the theme file <code>river_dreams.zsh-theme</code> that is in the repository's root directory that you have just downloaded. Here is the rule to use:</li>
					<pre><code>source ~/.config/zsh/themes/river_dreams/river_dreams.zsh-theme</code></pre>
					<p>If you have chosen other directory in the last step, you have to change the path in the source rule to point to the directory you chose. Pay attention to use <code>/river_dreams.zsh-theme</code> in the end of the path to include the theme file instead of the directory itself.</p>
					<li>Restart your ZSH session.</li>
					<p>At this point, the source code will be automatically compiled and, when it finishes, the prompt will be ready for you to use.</p>
				</ul>
				<p>If you want to uninstall River Dreams, follow these steps:</p>
				<ul>
					<li>Remove the repository that you have cloned.</li>
					<pre><code>rm -rf  ${HOME}/.config/zsh/themes/river_dreams</code></pre>
					<p>This command will remove the directory used in the examples. If you have used other directory, you will have to change it in the command when you use it.</p>
					<li>Remove the source rule you have included in the <code>${HOME}/.zshrc</code> file.</li>
				</ul>
			<h4>Installation Within OhMyZSH</h4>
				<p>For these instructions to work, as expected, you must install OhMyZSH first. Installation instructions to do such can be found in the <a href="https://github.com/ohmyzsh/ohmyzsh">repository</a> of the project on GitHub.</p>
				<ul>
					<li>Download this repository to the directory where OhMyZSH looks for custom themes.</li>
					<p>If you installed <code>git</code>, this command will do the trick:</p>
					<pre><code>git clone --depth=1 https://github.com/skippyr/river_dreams ${ZSH_CUSTOM:-${HOME}/.oh-my-zsh/custom}/themes/river_dreams</code></pre>
					<p>This command will clone this repository to the required directory. The flag <code>--depth</code> with value <code>1</code> specifies to <code>git</code> that you only want to download the last commit, instead of the whole commit tree as it does by default.</p>
					<p>If you did not install <code>git</code>, you can download the repository from its page on GitHub. Just access the page of the project there, then click on the button labeled <code>Code</code> on the top of the page, then click on the button labeled <code>Download ZIP</code> that will be in the floating menu that appears. This will download a ZIP file, you just have to unzip it in the directory you want it to be.</p>
					<li>Change the value of the variable <code>ZSH_THEME</code> in the <code>${HOME}/.zshrc</code> file to start River Dreams:</li>
					<pre><code>ZSH_THEME="river_dreams/river_dreams"</code></pre>
					<li>Restart your ZSH session.</li>
					<p>At this point, the source code will be automatically compiled and, when it finishes, the prompt will be ready for you to use.</p>
				</ul>
				<p>If you want to uninstall River Dreams, follow these steps:</p>
				<ul>
					<li>Remove the repository that you have cloned.</li>
					<pre><code>rm -rf ${ZSH_CUSTOM:-${HOME}/.oh-my-zsh/custom}/themes/river_dreams</code></pre>
					<li>Change the value of the variable <code>ZSH_THEME</code> in the <code>${HOME}/.zshrc</code> file for a valid theme from OhMyZSH. Its default value is:</li>
					<pre><code>ZSH_THEME="robbyrussell"</code></pre>
				</ul>
		<h2>Customization</h2>
			<p>You can enable the fallback feature of River Dreams by using an environment variable called <code>RIVER_DREAMS_USE_FALLBACK_TEXT</code>. That variable expects an integer value that represents a boolean:</p>
			<ul>
				<li><code>0</code> means <code>false</code>, which will deativate the feature.</li>
				<pre><code>export RIVER_DREAMS_USE_FALLBACK_TEXT="0"</code></pre>
				<li><code>1</code> means <code>true</code>, which will enable the feature.</li>
				<pre><code>export RIVER_DREAMS_USE_FALLBACK_TEXT="1"</code></pre>
			</ul>
			<p>If not set, River Dreams will automatically set it a value based on your terminal emulator's color support. I know that it may sound weird, but knowing if your terminal emulator's can support the 8-bits palette or not really does the tricky as there is not better way to check if your terminal can render pretty symbols.</p>
			<p>Further more, River Dreams does not offers another user-friendly way of changing its look. If you need to tweak it, you will need to change its source code, then recompile it again. If that is your case, the shell script that you source to start it exposes a function to compile the code easily, which you can use:</p>
			<pre><code>river_dreams::compile</code></pre>
		<h2>Updates</h2>
			<p>This project receives updates frequently to fix bugs, add new features and more. Keeping your local copy of it up-to-date is a good idea.</p>
			<p>If you installed <code>git</code>, to update it, you will need to go back to the directory where you have stored it in your machine and run the following command:</p>
			<pre><code>git pull origin main</code></pre>
			<p>If you did not installed <code>git</code>, you will have to download the repository again and replace the directory that you have.</p>
			<p>After pulling the new updates, use the following function in your shell to compile the source code again.</p>
			<pre><code>river_dreams::compile</code></pre>
		<h2>See Also</h2>
			<p>Thanks to <a href="https://github.com/unixorn">unixorn</a>, River Dreams is now available in the <a href="https://github.com/unixorn/awesome-zsh-plugins">awesome-zsh-plugins</a> repository. It hosts a lot of other cool ZSH themes, plugins, frameworks and more. So what about going there to see what else you like?</p>
		<h2>Issues And Contributions</h2>
			<p>Learn how to report issues, questions and ideas and how to contribute to this project by reading its <a href="https://skippyr.github.io/materials/pages/contributions_guideline.html">contributions guideline</a>.</p>
		<h2>License</h2>
			<p>River Dreams is released under the terms of the MIT License. You can refer to the license as the file <code><a href="https://github.com/skippyr/river_dreams/blob/main/LICENSE">LICENSE</a></code> in the root directory of this repository.</p>
			<p>Copyright (c) 2023, Sherman Rofeman. MIT License.</p>

