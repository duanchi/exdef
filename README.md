##Exdef - External Definition
====

A External Definition Extension for PHP

### Requirement
- PHP 7+

### Introduction

Exdef is a replacement of the function "define" in PHP, as define constant in the initialization before PHP application applying or improve performance of initialization defining in PHP application.

###Features
- Fast, Light
- Support namespace.
- One file to config.

### Install

#### Compile Exdef in Linux
```
$ /path/to/php7/phpize
$ ./configure --with-php-config=/path/to/php7/php-config/
$ make && make install
```

### Runtime Configure
- exdef.ini_file    // file to load which constants to init.


### Example

#### Ini_file
 
php.ini or other ini file which PHP can load at PHP start. scope `PHP_INI_SYSTEM`

````ini
exdef.ini_file=/tmp/exdef.ini
````

#### INI File

````ini
foo.me = "duanchi"
foo.my_wife = "fate"
bar = "exdef"
boolean = true
````
#### Run

Run your php codes.


````php
php7 -r 'var_dump(get_defined_constants(true));'

/*
...
["exdef"]=>
  array(4) {
    ["FOO\ME"]=>
    string(7) "duanchi"
    ["FOO\MY_WIFE"]=>
    string(4) "fate"
    ["BAR"]=>
    string(5) "exdef"
    ["boolean"]=>
    string(1) "1"
  }
...  
*/
````


