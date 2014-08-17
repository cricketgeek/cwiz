P=pthreads
objects=string_utilities.objects
CFLAGS=`pkg-config --cflags glib-2.0` -g -Wall -std=gnu99 -03 -pthread
LDLIBS=`pkg-config --libs glib-2.0` -lpthread

$(P): $(objects)