"BEGIN {\n"
"$INC{'Xchat.pm'} = 'DUMMY';\n"
"}\n"
"\n"
"BEGIN {\n"
"$SIG{__WARN__} = sub {\n"
"my $message = shift @_;\n"
"my ($package) = caller;\n"
"my $pkg_info = Xchat::Embed::pkg_info( $package );\n"
"\n"
"\n"
"if( $message =~ /^(?:Gtk|GLib|Gdk)(?:-\\w+)?-(?:ERROR|CRITICAL|WARNING|MESSAGE|INFO|DEBUG)/i ) {\n"
"print STDERR $message;\n"
"} else {\n"
"\n"
"if( $pkg_info ) {\n"
"if( $message =~ /\\(eval 1\\)/ ) {\n"
"$message =~ s/\\(eval 1\\)/(PERL PLUGIN CODE)/;\n"
"} else {\n"
"$message =~ s/\\(eval \\d+\\)/$pkg_info->{filename}/;\n"
"}\n"
"}\n"
"\n"
"Xchat::print( $message );\n"
"}\n"
"};\n"
"}\n"
"use File::Spec ();\n"
"use File::Basename ();\n"
"use File::Glob ();\n"
"use Symbol();\n"
"\n"
"{\n"
"package Xchat;\n"
"use base qw(Exporter);\n"
"use strict;\n"
"use warnings;\n"
"\n"
"our %EXPORT_TAGS = (\n"
"constants => [\n"
"qw(PRI_HIGHEST PRI_HIGH PRI_NORM PRI_LOW PRI_LOWEST), # priorities\n"
"qw(EAT_NONE EAT_XCHAT EAT_PLUGIN EAT_ALL), # callback return values\n"
"qw(FD_READ FD_WRITE FD_EXCEPTION FD_NOTSOCKET), # fd flags\n"
"qw(KEEP REMOVE), # timers\n"
"],\n"
"hooks => [\n"
"qw(hook_server hook_command hook_print hook_timer unhook),\n"
"],\n"
"util => [\n"
"qw(register nickcmp strip_code), # misc\n"
"qw(print prnt printf prntf command commandf emit_print), # output\n"
"qw(find_context get_context set_context), # context\n"
"qw(get_info get_prefs get_list context_info user_info), # input\n"
"],\n"
");\n"
"\n"
"$EXPORT_TAGS{all} = [ map { @{$_} } @EXPORT_TAGS{qw(constants hooks util)}];\n"
"our @EXPORT = @{$EXPORT_TAGS{constants}};\n"
"our @EXPORT_OK = @{$EXPORT_TAGS{all}};\n"
"\n"
"sub register {\n"
"my $package = Xchat::Embed::find_pkg();\n"
"my $pkg_info = Xchat::Embed::pkg_info( $package );\n"
"my $filename = $pkg_info->{filename};\n"
"my ($name, $version, $description, $callback) = @_;\n"
"$description = \"\" unless defined $description;\n"
"$pkg_info->{shutdown} = $callback;\n"
"\n"
"$pkg_info->{gui_entry} =\n"
"Xchat::Internal::register( $name, $version, $description, $filename );\n"
"\n"
"return ();\n"
"}\n"
"\n"
"sub hook_server {\n"
"return undef unless @_ >= 2;\n"
"my $message = shift;\n"
"my $callback = shift;\n"
"my $options = shift;\n"
"my $package = Xchat::Embed::find_pkg();\n"
"\n"
"$callback = Xchat::Embed::fix_callback( $package, $callback );\n"
"\n"
"my ($priority, $data) = ( Xchat::PRI_NORM, undef );\n"
"\n"
"if( ref( $options ) eq 'HASH' ) {\n"
"if( exists( $options->{priority} ) && defined( $options->{priority} ) ) {\n"
"$priority = $options->{priority};\n"
"}\n"
"\n"
"if( exists( $options->{data} ) && defined( $options->{data} ) ) {\n"
"$data = $options->{data};\n"
"}\n"
"}\n"
"\n"
"my $pkg_info = Xchat::Embed::pkg_info( $package );\n"
"my $hook = Xchat::Internal::hook_server(\n"
"$message, $priority, $callback, $data\n"
");\n"
"push @{$pkg_info->{hooks}}, $hook if defined $hook;\n"
"return $hook;\n"
"}\n"
"\n"
"sub hook_command {\n"
"return undef unless @_ >= 2;\n"
"my $command = shift;\n"
"my $callback = shift;\n"
"my $options = shift;\n"
"my $package = Xchat::Embed::find_pkg();\n"
"\n"
"$callback = Xchat::Embed::fix_callback( $package, $callback );\n"
"\n"
"my ($priority, $help_text, $data) = ( Xchat::PRI_NORM, undef, undef );\n"
"\n"
"if( ref( $options ) eq 'HASH' ) {\n"
"if( exists( $options->{priority} ) && defined( $options->{priority} ) ) {\n"
"$priority = $options->{priority};\n"
"}\n"
"\n"
"if(\n"
"exists( $options->{help_text} )\n"
"&& defined( $options->{help_text} )\n"
") {\n"
"$help_text = $options->{help_text};\n"
"}\n"
"\n"
"if ( exists( $options->{data} ) && defined( $options->{data} ) ) {\n"
"$data = $options->{data};\n"
"}\n"
"}\n"
"\n"
"my $pkg_info = Xchat::Embed::pkg_info( $package );\n"
"my $hook = Xchat::Internal::hook_command(\n"
"$command, $priority, $callback, $help_text, $data\n"
");\n"
"push @{$pkg_info->{hooks}}, $hook if defined $hook;\n"
"return $hook;\n"
"}\n"
"\n"
"sub hook_print {\n"
"return undef unless @_ >= 2;\n"
"my $event = shift;\n"
"my $callback = shift;\n"
"my $options = shift;\n"
"my $package = Xchat::Embed::find_pkg();\n"
"\n"
"$callback = Xchat::Embed::fix_callback( $package, $callback );\n"
"\n"
"my ($priority, $data) = ( Xchat::PRI_NORM, undef );\n"
"\n"
"if ( ref( $options ) eq 'HASH' ) {\n"
"if ( exists( $options->{priority} ) && defined( $options->{priority} ) ) {\n"
"$priority = $options->{priority};\n"
"}\n"
"\n"
"if ( exists( $options->{data} ) && defined( $options->{data} ) ) {\n"
"$data = $options->{data};\n"
"}\n"
"}\n"
"\n"
"my $pkg_info = Xchat::Embed::pkg_info( $package );\n"
"my $hook = Xchat::Internal::hook_print(\n"
"$event, $priority, $callback, $data\n"
");\n"
"push @{$pkg_info->{hooks}}, $hook if defined $hook;\n"
"return $hook;\n"
"}\n"
"\n"
"sub hook_timer {\n"
"return undef unless @_ >= 2;\n"
"my ($timeout, $callback, $data) = @_;\n"
"my $package = Xchat::Embed::find_pkg();\n"
"\n"
"$callback = Xchat::Embed::fix_callback( $package, $callback );\n"
"\n"
"if(\n"
"ref( $data ) eq 'HASH' && exists( $data->{data} )\n"
"&& defined( $data->{data} )\n"
") {\n"
"$data = $data->{data};\n"
"}\n"
"\n"
"my $pkg_info = Xchat::Embed::pkg_info( $package );\n"
"my $hook = Xchat::Internal::hook_timer( $timeout, $callback, $data );\n"
"push @{$pkg_info->{hooks}}, $hook if defined $hook;\n"
"return $hook;\n"
"}\n"
"\n"
"sub hook_fd {\n"
"return undef unless @_ >= 2;\n"
"my ($fd, $callback, $options) = @_;\n"
"return undef unless defined $fd && defined $callback;\n"
"\n"
"my $fileno = fileno $fd;\n"
"return undef unless defined $fileno; # no underlying fd for this handle\n"
"\n"
"my ($package) = Xchat::Embed::find_pkg();\n"
"$callback = Xchat::Embed::fix_callback( $package, $callback );\n"
"\n"
"my ($flags, $data) = (Xchat::FD_READ, undef);\n"
"\n"
"if( ref( $options ) eq 'HASH' ) {\n"
"if( exists( $options->{flags} ) && defined( $options->{flags} ) ) {\n"
"$flags = $options->{flags};\n"
"}\n"
"\n"
"if ( exists( $options->{data} ) && defined( $options->{data} ) ) {\n"
"$data = $options->{data};\n"
"}\n"
"}\n"
"\n"
"my $cb = sub {\n"
"my $userdata = shift;\n"
"no strict 'refs';\n"
"return &{$userdata->{CB}}(\n"
"$userdata->{FD}, $userdata->{FLAGS}, $userdata->{DATA},\n"
");\n"
"};\n"
"\n"
"my $pkg_info = Xchat::Embed::pkg_info( $package );\n"
"my $hook = Xchat::Internal::hook_fd(\n"
"$fileno, $cb, $flags, {\n"
"DATA => $data, FD => $fd, CB => $callback, FLAGS => $flags,\n"
"}\n"
");\n"
"push @{$pkg_info->{hooks}}, $hook if defined $hook;\n"
"return $hook;\n"
"}\n"
"\n"
"sub unhook {\n"
"my $hook = shift @_;\n"
"my $package = shift @_;\n"
"($package) = caller unless $package;\n"
"my $pkg_info = Xchat::Embed::pkg_info( $package );\n"
"\n"
"if( $hook =~ /^\\d+$/ && grep { $_ == $hook } @{$pkg_info->{hooks}} ) {\n"
"$pkg_info->{hooks} = [grep { $_ != $hook } @{$pkg_info->{hooks}}];\n"
"return Xchat::Internal::unhook( $hook );\n"
"}\n"
"return ();\n"
"}\n"
"\n"
"sub do_for_each {\n"
"my ($cb, $channels, $servers) = @_;\n"
"\n"
"\n"
"\n"
"unless( $channels or $servers ) {\n"
"$cb->();\n"
"return 1;\n"
"}\n"
"\n"
"$channels = [ $channels ] unless ref( $channels ) eq 'ARRAY';\n"
"\n"
"\n"
"\n"
"if( $servers ) {\n"
"$servers = [ $servers ] unless ref( $servers ) eq 'ARRAY';\n"
"} else {\n"
"$servers = [ Xchat::get_info( \"server\" ) ];\n"
"}\n"
"\n"
"my $num_done;\n"
"my $old_ctx = Xchat::get_context();\n"
"for my $server ( @$servers ) {\n"
"for my $channel ( @$channels ) {\n"
"my $old_ctx = Xchat::get_context();\n"
"my $ctx = Xchat::find_context( $channel, $server );\n"
"\n"
"if( $ctx ) {\n"
"Xchat::set_context( $ctx );\n"
"$cb->();\n"
"$num_done++\n"
"}\n"
"}\n"
"}\n"
"Xchat::set_context( $old_ctx );\n"
"return $num_done;\n"
"}\n"
"\n"
"sub print {\n"
"my $text = shift @_;\n"
"return \"\" unless defined $text;\n"
"if( ref( $text ) eq 'ARRAY' ) {\n"
"if( $, ) {\n"
"$text = join $, , @$text;\n"
"} else {\n"
"$text = join \"\", @$text;\n"
"}\n"
"}\n"
"\n"
"return do_for_each(\n"
"sub { Xchat::Internal::print( $text ); },\n"
"@_\n"
");\n"
"}\n"
"\n"
"sub printf {\n"
"my $format = shift;\n"
"Xchat::print( sprintf( $format, @_ ) );\n"
"}\n"
"\n"
"\n"
"\n"
"*Xchat::prnt = *Xchat::print{CODE};\n"
"*Xchat::prntf = *Xchat::printf{CODE};\n"
"\n"
"sub command {\n"
"my $command = shift;\n"
"return \"\" unless defined $command;\n"
"my @commands;\n"
"\n"
"if( ref( $command ) eq 'ARRAY' ) {\n"
"@commands = @$command;\n"
"} else {\n"
"@commands = ($command);\n"
"}\n"
"\n"
"return do_for_each(\n"
"sub { Xchat::Internal::command( $_ ) foreach @commands },\n"
"@_\n"
");\n"
"}\n"
"\n"
"sub commandf {\n"
"my $format = shift;\n"
"Xchat::command( sprintf( $format, @_ ) );\n"
"}\n"
"\n"
"sub set_context {\n"
"my $context;\n"
"if( @_ == 2 ) {\n"
"my ($channel, $server) = @_;\n"
"$context = Xchat::find_context( $channel, $server );\n"
"} elsif( @_ == 1 ) {\n"
"if( defined $_[0] && $_[0] =~ /^\\d+$/ ) {\n"
"$context = $_[0];\n"
"} else {\n"
"$context = Xchat::find_context( $_[0] );\n"
"}\n"
"}\n"
"return $context ? Xchat::Internal::set_context( $context ) : 0;\n"
"}\n"
"\n"
"sub get_info {\n"
"my $id = shift;\n"
"my $info;\n"
"\n"
"if( defined( $id ) ) {\n"
"if( grep { $id eq $_ } qw(state_cursor id) ) {\n"
"$info = Xchat::get_prefs( $id );\n"
"} else {\n"
"$info = Xchat::Internal::get_info( $id );\n"
"}\n"
"}\n"
"return $info;\n"
"}\n"
"\n"
"sub user_info {\n"
"my $nick = Xchat::strip_code(shift @_ || Xchat::get_info( \"nick\" ));\n"
"my $user;\n"
"for (Xchat::get_list( \"users\" ) ) {\n"
"if ( Xchat::nickcmp( $_->{nick}, $nick ) == 0 ) {\n"
"$user = $_;\n"
"last;\n"
"}\n"
"}\n"
"return $user;\n"
"}\n"
"\n"
"sub context_info {\n"
"my $ctx = shift @_ || Xchat::get_context;\n"
"my $old_ctx = Xchat::get_context;\n"
"my @fields = (\n"
"qw(away channel charset host id inputbox libdirfs network),\n"
"qw(nick nickserv server topic version win_status xchatdir xchatdirfs),\n"
"qw(state_cursor),\n"
");\n"
"\n"
"if( Xchat::set_context( $ctx ) ) {\n"
"my %info;\n"
"for my $field ( @fields ) {\n"
"$info{$field} = Xchat::get_info( $field );\n"
"}\n"
"\n"
"my $ctx_info = Xchat::Internal::context_info;\n"
"@info{keys %$ctx_info} = values %$ctx_info;\n"
"\n"
"Xchat::set_context( $old_ctx );\n"
"return %info if wantarray;\n"
"return \\%info;\n"
"} else {\n"
"return undef;\n"
"}\n"
"}\n"
"\n"
"sub strip_code {\n"
"my $pattern = qr[\n"
"\\cB| #Bold\n"
"\\cC\\d{0,2}(?:,\\d{1,2})?| #Color\n"
"\\cG| #Beep\n"
"\\cO| #Reset\n"
"\\cV| #Reverse\n"
"\\c_  #Underline\n"
"]x;\n"
"\n"
"if( defined wantarray ) {\n"
"my $msg = shift;\n"
"$msg =~ s/$pattern//g;\n"
"return $msg;\n"
"} else {\n"
"$_[0] =~ s/$pattern//g;\n"
"}\n"
"}\n"
"\n"
"} # end of Xchat package\n"
"\n"
"{\n"
"package Xchat::Embed;\n"
"use strict;\n"
"use warnings;\n"
"\n"
"our %scripts;\n"
"\n"
"sub load {\n"
"my $file = expand_homedir( shift @_ );\n"
"my $package = file2pkg( $file );\n"
"\n"
"if( exists $scripts{$package} ) {\n"
"my $pkg_info = pkg_info( $package );\n"
"my $filename = File::Basename::basename( $pkg_info->{filename} );\n"
"Xchat::print(\n"
"qq{'$filename' already loaded from '$pkg_info->{filename}'.\\n}\n"
");\n"
"Xchat::print(\n"
"'If this is a different script then it rename and try '.\n"
"'loading it again.'\n"
");\n"
"return 2;\n"
"}\n"
"\n"
"if( open FH, $file ) {\n"
"my $source = do {local $/; <FH>};\n"
"close FH;\n"
"\n"
"$source =~ s/^__END__.*//ms;\n"
"\n"
"if(\n"
"my @replacements = $source =~\n"
"m/^\\s*package ((?:[^\\W:]+(?:::)?)+)\\s*?;/mg\n"
") {\n"
"\n"
"if ( @replacements > 1 ) {\n"
"Xchat::print(\n"
"\"Too many package defintions, only 1 is allowed\\n\"\n"
");\n"
"return 1;\n"
"}\n"
"\n"
"my $original_package = shift @replacements;\n"
"\n"
"\n"
"$source =~ s/^(package $original_package\\s*;)/#$1/m;\n"
"\n"
"\n"
"$source =~ s/${original_package}:://g;\n"
"}\n"
"\n"
"\n"
"\n"
"$scripts{$package}{filename} = $file;\n"
"\n"
"{\n"
"no strict; no warnings;\n"
"eval \"package $package; $source;\";\n"
"\n"
"unless( exists $scripts{$package}{gui_entry} ) {\n"
"$scripts{$package}{gui_entry} =\n"
"Xchat::Internal::register(\n"
"\"???\", \"???\", \"This script did not call register()\", $file\n"
");\n"
"}\n"
"}\n"
"\n"
"if( $@ ) {\n"
"\n"
"Xchat::print( \"Error loading '$file':\\n$@\\n\" );\n"
"\n"
"unload( $scripts{$package}{filename} );\n"
"return 1;\n"
"}\n"
"} else {\n"
"Xchat::print( \"Error opening '$file': $!\\n\" );\n"
"return 2;\n"
"}\n"
"\n"
"return 0;\n"
"}\n"
"\n"
"sub unload {\n"
"my $file = shift @_;\n"
"my $package = file2pkg( $file );\n"
"my $pkg_info = pkg_info( $package );\n"
"\n"
"if( $pkg_info ) {	\n"
"\n"
"if( exists $pkg_info->{shutdown} ) {\n"
"\n"
"eval {\n"
"if( ref $pkg_info->{shutdown} eq 'CODE' ) {\n"
"$pkg_info->{shutdown}->();\n"
"} elsif ( $pkg_info->{shutdown} ) {\n"
"no strict 'refs';\n"
"&{$pkg_info->{shutdown}};\n"
"}\n"
"};\n"
"}\n"
"\n"
"if( exists $pkg_info->{hooks} ) {\n"
"for my $hook ( @{$pkg_info->{hooks}} ) {\n"
"Xchat::unhook( $hook, $package );\n"
"}\n"
"}\n"
"\n"
"\n"
"if( exists $pkg_info->{gui_entry} ) {\n"
"plugingui_remove( $pkg_info->{gui_entry} );\n"
"}\n"
"\n"
"Symbol::delete_package( $package );\n"
"delete $scripts{$package};\n"
"return Xchat::EAT_ALL;\n"
"} else {\n"
"Xchat::print( qq{\"$file\" is not loaded.\\n} );\n"
"return Xchat::EAT_NONE;\n"
"}\n"
"}\n"
"\n"
"sub unload_all {\n"
"for my $package ( keys %scripts ) {\n"
"unload( $scripts{$package}->{filename} );\n"
"}\n"
"\n"
"return Xchat::EAT_ALL;\n"
"}\n"
"\n"
"sub reload {\n"
"my $file = shift @_;\n"
"my $package = file2pkg( $file );\n"
"my $pkg_info = pkg_info( $package );\n"
"my $fullpath = $file;\n"
"\n"
"if( $pkg_info ) {\n"
"$fullpath = $pkg_info->{filename};\n"
"unload( $file );\n"
"}\n"
"\n"
"load( $fullpath );\n"
"return Xchat::EAT_ALL;\n"
"}\n"
"\n"
"sub reload_all {\n"
"my $dir = Xchat::get_info( \"xchatdirfs\" ) || Xchat::get_info( \"xchatdir\" );\n"
"my $auto_load_glob = File::Spec->catfile( $dir, \"*.pl\" );\n"
"my @scripts = map { $_->{filename} } values %scripts;\n"
"push @scripts, File::Glob::bsd_glob( $auto_load_glob );\n"
"\n"
"my %loaded;\n"
"for my $script ( @scripts ) {\n"
"next if $loaded{ $script };\n"
"reload( $script );\n"
"$loaded{ $script }++;\n"
"}\n"
"}\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"sub expand_homedir {\n"
"my $file = shift @_;\n"
"\n"
"if ( $^O eq \"MSWin32\" ) {\n"
"$file =~ s/^~/$ENV{USERPROFILE}/;\n"
"} else {\n"
"$file =~ s{^~}{\n"
"(getpwuid($>))[7] ||  $ENV{HOME} || $ENV{LOGDIR}\n"
"}ex;\n"
"}\n"
"return $file;\n"
"}\n"
"\n"
"sub file2pkg {\n"
"my $string = File::Basename::basename( shift @_ );\n"
"$string =~ s/\\.pl$//i;\n"
"$string =~ s|([^A-Za-z0-9/])|'_'.unpack(\"H*\",$1)|eg;\n"
"return \"Xchat::Script::\" . $string;\n"
"}\n"
"\n"
"sub pkg_info {\n"
"my $package = shift @_;\n"
"return $scripts{$package};\n"
"}\n"
"\n"
"sub find_pkg {\n"
"my $level = 1;\n"
"my $package = (caller( $level ))[0];\n"
"while( $package !~ /^Xchat::Script::/ ) {\n"
"$level++;\n"
"$package = (caller( $level ))[0];\n"
"}\n"
"return $package;\n"
"}\n"
"\n"
"sub fix_callback {\n"
"my ($package, $callback) = @_;\n"
"\n"
"unless( ref $callback ) {\n"
"\n"
"$callback =~ s/^.*:://;\n"
"$callback = qq[${package}::$callback];\n"
"}\n"
"\n"
"return $callback;\n"
"}\n"
"} # end of Xchat::Embed package\n"
#ifdef OLD_PERL
"{ package IRC;\n"
"sub IRC::register {\n"
"my ($script_name, $version, $callback) = @_;\n"
"my $package = caller;\n"
"$callback = Xchat::Embed::fix_callback( $package, $callback) if $callback;\n"
"Xchat::register( $script_name, $version, undef, $callback );\n"
"}\n"
"\n"
"\n"
"sub IRC::add_command_handler {\n"
"my ($command, $callback) = @_;\n"
"my $package = caller;\n"
"\n"
"$callback = Xchat::Embed::fix_callback( $package, $callback );\n"
"\n"
"\n"
"\n"
"my $start_index = $command ? 1 : 0;\n"
"\n"
"Xchat::hook_command( $command,\n"
"sub {\n"
"no strict 'refs';\n"
"return &{$callback}($_[1][$start_index]);\n"
"}\n"
");\n"
"return;\n"
"}\n"
"\n"
"sub IRC::add_message_handler {\n"
"my ($message, $callback) = @_;\n"
"my $package = caller;\n"
"$callback = Xchat::Embed::fix_callback( $package, $callback );\n"
"\n"
"Xchat::hook_server( $message,\n"
"sub {\n"
"no strict 'refs';\n"
"return &{$callback}( $_[1][0] );\n"
"}\n"
");\n"
"return;\n"
"}\n"
"\n"
"sub IRC::add_print_handler {\n"
"my ($event, $callback) = @_;\n"
"my $package = caller;\n"
"$callback = Xchat::Embed::fix_callback( $package, $callback );\n"
"Xchat::hook_print( $event,\n"
"sub {\n"
"my @word = @{$_[0]};\n"
"no strict 'refs';\n"
"return &{$callback}( join( ' ', @word[0..3] ), @word );\n"
"}\n"
");\n"
"return;\n"
"}\n"
"\n"
"sub IRC::add_timeout_handler {\n"
"my ($timeout, $callback) = @_;\n"
"my $package = caller;\n"
"$callback = Xchat::Embed::fix_callback( $package, $callback );\n"
"Xchat::hook_timer( $timeout,\n"
"sub {\n"
"no strict 'refs';\n"
"&{$callback};\n"
"return 0;\n"
"}\n"
");\n"
"return;\n"
"}\n"
"\n"
"sub IRC::command {\n"
"my $command = shift;\n"
"if( $command =~ m{^/} ) {\n"
"$command =~ s{^/}{};\n"
"Xchat::command( $command );\n"
"} else {\n"
"Xchat::command( qq[say $command] );\n"
"}\n"
"}\n"
"\n"
"sub IRC::command_with_channel {\n"
"my ($command, $channel, $server) = @_;\n"
"my $old_ctx = Xchat::get_context;\n"
"my $ctx = Xchat::find_context( $channel, $server );\n"
"\n"
"if( $ctx ) {\n"
"Xchat::set_context( $ctx );\n"
"IRC::command( $command );\n"
"Xchat::set_context( $ctx );\n"
"}\n"
"}\n"
"\n"
"sub IRC::command_with_server {\n"
"my ($command, $server) = @_;\n"
"my $old_ctx = Xchat::get_context;\n"
"my $ctx = Xchat::find_context( undef, $server );\n"
"\n"
"if( $ctx ) {\n"
"Xchat::set_context( $ctx );\n"
"IRC::command( $command );\n"
"Xchat::set_context( $ctx );\n"
"}\n"
"}\n"
"\n"
"sub IRC::dcc_list {\n"
"my @dccs;\n"
"for my $dcc ( Xchat::get_list( 'dcc' ) ) {\n"
"push @dccs, $dcc->{nick};\n"
"push @dccs, $dcc->{file} ? $dcc->{file} : '';\n"
"push @dccs, @{$dcc}{qw(type status cps size)};\n"
"push @dccs, $dcc->{type} == 0 ? $dcc->{pos} : $dcc->{resume};\n"
"push @dccs, $dcc->{address32};\n"
"push @dccs, $dcc->{destfile} ? $dcc->{destfile} : '';\n"
"}\n"
"return @dccs;\n"
"}\n"
"\n"
"sub IRC::channel_list {\n"
"my @channels;\n"
"for my $channel ( Xchat::get_list( 'channels' ) ) {\n"
"push @channels, @{$channel}{qw(channel server)},\n"
"Xchat::context_info( $channel->{context} )->{nick};\n"
"}\n"
"return @channels;\n"
"}\n"
"\n"
"sub IRC::get_info {\n"
"my $id = shift;\n"
"my @ids = qw(version nick channel server xchatdir away network host topic);\n"
"\n"
"if( $id >= 0 && $id <= 8 && $id != 5 ) {\n"
"my $info = Xchat::get_info($ids[$id]);\n"
"return defined $info ? $info : '';\n"
"} else {\n"
"if( $id == 5 ) {\n"
"return Xchat::get_info( 'away' ) ? 1 : 0;\n"
"} else {\n"
"return 'Error2';\n"
"}\n"
"}\n"
"}\n"
"\n"
"sub IRC::get_prefs {\n"
"return 'Unknown variable' unless defined $_[0];\n"
"my $result = Xchat::get_prefs(shift);\n"
"return defined $result ? $result : 'Unknown variable';\n"
"}\n"
"\n"
"sub IRC::ignore_list {\n"
"my @ignores;\n"
"for my $ignore ( Xchat::get_list( 'ignore' ) ) {\n"
"push @ignores, $ignore->{mask};\n"
"my $flags = $ignore->{flags};\n"
"push @ignores, $flags & 1, $flags & 2, $flags & 4, $flags & 8, $flags & 16,\n"
"$flags & 32, ':';\n"
"}\n"
"return @ignores;\n"
"}\n"
"\n"
"sub IRC::print {\n"
"Xchat::print( $_ ) for @_;\n"
"return;\n"
"}\n"
"\n"
"sub IRC::print_with_channel {\n"
"Xchat::print( @_ );\n"
"}\n"
"\n"
"sub IRC::send_raw {\n"
"Xchat::commandf( qq[quote %s], shift );\n"
"}\n"
"\n"
"sub IRC::server_list {\n"
"my @servers;\n"
"for my $channel ( Xchat::get_list( 'channels' ) ) {\n"
"push @servers, $channel->{server} if $channel->{server};\n"
"}\n"
"return @servers;\n"
"}\n"
"\n"
"sub IRC::user_info {\n"
"my $user;\n"
"if( @_ > 0 ) {\n"
"$user = Xchat::user_info( shift );\n"
"} else {\n"
"$user = Xchat::user_info();\n"
"}\n"
"\n"
"my @info;\n"
"if( $user ) {\n"
"push @info, $user->{nick};\n"
"if( $user->{host} ) {\n"
"push @info, $user->{host};\n"
"} else {\n"
"push @info, 'FETCHING';\n"
"}\n"
"push @info, $user->{prefix} eq '@' ? 1 : 0;\n"
"push @info, $user->{prefix} eq '+' ? 1 : 0;\n"
"}\n"
"return @info;\n"
"}\n"
"\n"
"sub IRC::user_list {\n"
"my ($channel, $server) = @_;\n"
"my $ctx = Xchat::find_context( $channel, $server );\n"
"my $old_ctx = Xchat::get_context;\n"
"\n"
"if( $ctx ) {\n"
"Xchat::set_context( $ctx );\n"
"my @users;\n"
"for my $user ( Xchat::get_list( 'users' ) ) {\n"
"push @users, $user->{nick};\n"
"if( $user->{host} ) {\n"
"push @users, $user->{host};\n"
"} else {\n"
"push @users, 'FETCHING';\n"
"}\n"
"push @users, $user->{prefix} eq '@' ? 1 : 0;\n"
"push @users, $user->{prefix} eq '+' ? 1 : 0;\n"
"push @users, ':';\n"
"}\n"
"Xchat::set_context( $old_ctx );\n"
"return @users;\n"
"} else {\n"
"return;\n"
"}\n"
"}\n"
"\n"
"sub IRC::user_list_short {\n"
"my ($channel, $server) = @_;\n"
"my $ctx = Xchat::find_context( $channel, $server );\n"
"my $old_ctx = Xchat::get_context;\n"
"\n"
"if( $ctx ) {\n"
"Xchat::set_context( $ctx );\n"
"my @users;\n"
"for my $user ( Xchat::get_list( 'users' ) ) {\n"
"my $nick = $user->{nick};\n"
"my $host = $user->{host} || 'FETCHING';\n"
"push @users, $nick, $host;\n"
"}\n"
"Xchat::set_context( $old_ctx );\n"
"return @users;\n"
"} else {\n"
"return;\n"
"}\n"
"\n"
"}\n"
"\n"
"sub IRC::add_user_list {}\n"
"sub IRC::sub_user_list {}\n"
"sub IRC::clear_user_list {}\n"
"sub IRC::notify_list {}\n"
"sub IRC::perl_script_list {}\n"
"}\n"
#endif