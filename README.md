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
			<p>Normally, this is enough to make your terminal emulators to use ZSH instead of Bash. However, there are some terminal emulators that will have a special settings to configure it inside them, which will overwrite your system's default: like Gnome Terminal and Konsole do.</p>
			<p>If that is your case, you can go to their settings, and search for a section where you can choose the command to run as the startup shell. Use <code>/bin/zsh</code> to initiate ZSH.</p>

