REPLACE INTO `command` (`name`, `permission`, `help`) VALUES ('caio', 5000, 'Syntax: .caio $subcommand\nType .caio to see the list of possible subcommands or .help caio $subcommand to see info on subcommands');
REPLACE INTO `command` (`name`, `permission`, `help`) VALUES ('caio forceresetall', 5000, 'Syntax: .caio resetall\r\n\r\nForce resets every player\'s addons. Player addons are deleted and downloaded again.');
REPLACE INTO `command` (`name`, `permission`, `help`) VALUES ('caio forcereset', 5000, 'Syntax: .caio reset [$playerName]\r\n\r\nForce resets player\'s addons. Player addons are deleted and downloaded again.');
REPLACE INTO `command` (`name`, `permission`, `help`) VALUES ('caio forcereload', 5000, 'Syntax: .caio reload [$playerName]\r\n\r\nForce reloads player\'s addons');
REPLACE INTO `command` (`name`, `permission`, `help`) VALUES ('caio forcereloadall', 5000, 'Syntax: .caio reloadall\r\n\r\nForce reloads every player\'s addons.');
REPLACE INTO `command` (`name`, `permission`, `help`) VALUES ('caio sendall', 5000, 'Syntax: .caio sendall "Message"\r\n\r\nSends an addon message to all players');
REPLACE INTO `command` (`name`, `permission`, `help`) VALUES ('caio send', 5000, 'Syntax: .caio send $playerName "Message"\r\n\r\nSends an addon message to player');
REPLACE INTO `command` (`name`, `permission`, `help`) VALUES ('caio version', 5000, 'Syntax: .caio version\r\n\r\nShows the AIO version');
REPLACE INTO `command` (`name`, `permission`, `help`) VALUES ('caio reloadaddons', 5000, 'Syntax: .caio reloadaddons\r\n\r\nReloads all client addons loaded on the server');
