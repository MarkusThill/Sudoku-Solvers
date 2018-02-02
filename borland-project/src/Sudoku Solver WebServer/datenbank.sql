CREATE TABLE 'sudoku' (
  'ID' int(10) unsigned NOT NULL auto_increment,
  'startsudoku' varchar(100) collate latin1_general_ci NOT NULL,
  'loesungsudoku' varchar(100) collate latin1_general_ci default NULL,
  'zeit' int(11) default NULL,
  PRIMARY KEY  ('ID')
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=92 ;