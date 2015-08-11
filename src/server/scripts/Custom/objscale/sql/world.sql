-- gameobject
ALTER TABLE `gameobject`ADD COLUMN `size` FLOAT NOT NULL DEFAULT '1' AFTER `state`;
UPDATE gameobject SET size = (SELECT size FROM gameobject_template WHERE entry = gameobject.id);
INSERT INTO `command` (`name`, `security`, `help`) VALUES ('gobject set scale', 2, 'Syntax: .gobject set scale #guid #scale\r\n\r\nGameobject with DB guid #guid size changed to #scale with related world vision update for players. Gameobject scale saved to DB and persistent.');

-- creature
ALTER TABLE `creature` ADD COLUMN `size` FLOAT NOT NULL DEFAULT '1' AFTER `dynamicflags`;
UPDATE creature SET size = (SELECT size FROM creature_template WHERE entry = creature.id);
INSERT INTO `command` (`name`, `security`, `help`) VALUES ('npc set scale', 2, 'Syntax: .npc set scale #scale\r\n\r\nSelected NPC size changed to #scale with related world vision update for players. NPC scale saved to DB and persistent.');
