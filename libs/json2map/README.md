[![Build Status](https://travis-ci.org/maximilianvoss/json2map.svg?branch=master)](https://travis-ci.org/maximilianvoss/json2map)
# Json2Map

Transforming JSON into a key/value map for easier access and processing in C.

Starting with a JSON object, looking like:

```
{  
   "test":{  
      "_id":{  
         "$oid":"566950d1afc4a3c1d86fcdfb"
      },
      "name":"picture",
      "file":"/var/www/html/pictureIn.png",
      "array":[  
         "1",
         "b",
         "3",
         "d",
         {  
            "object":"test"
         }
      ],
      "nullpointer":null,
      "number":1234,
      "true":true,
      "false":false
   }
}
```

Will be turned into a key/value pair looking like:
```
test._id.$oid = 566950d1afc4a3c1d86fcdfb
test.name = picture
test.file = /var/www/html/pictureIn.png
test.array[0] = 1
test.array[1] = b
test.array[2] = 3
test.array[3] = d
test.array[4].object = test
test.nullpointer = null
test.number = 1234
test.true = true
test.false = false
```

It also works the other way around. So your key/value map can be transformed to proper JSON again.
All keys and value are stored as strings. To check for primitives it might be easiest to check for digits and the values "null", "true" and "false". The representation of the key/value map can be changed easily as a *hook method* has to be defined and registered. So your key/value map representation can be used quite easily.  
For more details consult example.c

## Dependencies
This library depends on [csafestring](https://github.com/maximilianvoss/csafestring).
