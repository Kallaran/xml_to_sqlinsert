# xml_to_sqlinsert

The goal is to convert an xml file into a sql request of the form INSERT INTO.
The sql file can then be used to enter the data it contains into a database.


## some commands

Add the executable to an environment variable :
```
sudo cp xtsi /usr/local/bin/xtsi
```

The command below converts an xml file to a sql file. Be careful if the sql file already exists it will be rewritten.
```
xtsi file.xml file.sql
```


