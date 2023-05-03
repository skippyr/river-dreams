<h1>River Dreams</h1>
	<p>A fast multiline prompt for ZSH.</p>
	<h2>Installation And Usage</h2>
		<ul>
			<li>Install <a href="https://www.rust-lang.org/">Rust development tools</a>.</li>
			<p>These are the compiler and tools needed to compile River Dreams's source code.</p>
			<li>Download this repository to a directory in your machine.</li>
			<p>If you have <code>git</code> installed, you can download it in the following command:</p>
			<pre><code>git clone --depth 1 https://github.com/skippyr/river_dreams ~/.config/zsh/themes/river_dreams</code></pre>
			<p>This command will clone this repository to the directory <code>~/.config/zsh/themes/river_dreams</code>, however fell free to change it by the last command used in the example. Just remember the path you used because you will need it for the next step. The flag <code>--depth</code> with value <code>1</code> specifies to <code>git</code> that you only want to download the last commit, instead of the whole commit tree as it does by default.</p>
			<p>If you do not have <code>git</code> installed, do not worry, as you can also download the repository from GitHub. Just access the page of the project there, click on the <code>Code</code> button on the top of the page, then on <code>Download ZIP</code>. This will download a ZIP file, you just have to unzip it in the directory you want it to be.</p>
			<li>Add a source rule in your ZSH configuration file: <code>~/.zshrc</code> to source the theme file <code>river_dreams.zsh-theme</code> that is in the root directory of the repository that you have just downloaded.</li>
			<pre><code>source ~/.config/zsh/themes/river_dreams/river_dreams.zsh-theme</code></pre>
			<p>If you have choosen another directory in the last step, just change it in the source command. Pay attention to use <code>/river_dreams.zsh-theme</code> in the end of the path to include the theme file instead of the directory itself.</p>
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