# Awakener
## Intro

![in action](awakener.gif)

**THIS PROJECT IS NOT A PRODUCT** but rather an improvisation for the sake of learning and fun. Do not expect it to 
work for you or to be somewhat close to production quality.

### What
A simple HTTP server which allows to wake up a server that has Wake On Lan feature enabled.

### Why
In 2022, 2nd & 3rd of June were public holidays in the UK. 
It supposed to be only one holiday but this year, in addition to the Spring Bank Holiday, 
the country celebrated Queen's Platinum Jubilee. 
That left only three working days on that week, and on 1st of June I felt like I have unused energy to spend doing a 
silly project to have some fun.

### Tools

To make things interesting, I shall not use any of the tools I use day to day. I always liked C/C++ although, I 
don't have production experience using those, I have purchased a few books to learn them and the time has come to 
try something. I've tried modern C++ a few times already so decided to go with plain **C** this time.

I didn't want to mess around with tooling and environment setup, so I've chosen **Clion** IDE with an intention to see 
how it's doing (I've used it a few years ago) and with a hope it will sort most of the stuff out for me.

### Resources

* https://beej.us/guide/bgnet/html/ - great source of information
* https://www.manning.com/books/modern-c
* A bit of StackOverflow - not going to lie here, but I've tried to use it only as the last resort.


## Deploy

**TODO: there is for sure a room for improvement!**

Use `ReleaseAarch` target to build the binary on the Raspberry Pi server.
Note the target directory, it will be something like `/tmp/tmp.2gOYvaraa/cmake-build-releaseaarch`. 

*I couldn't figure out how to do cross-compilation on my Mac so here we are. Probably, should have used Docker for 
that tho.*

SSH to the server the copy files from it to the web folder:

```shell
cp BUILD_DIR/awakener /var/www/awakener
cp -r BUILD_DIR/public /var/www/awakener
sudo chown -R www-data:www-data /var/www/awakener
sudo chmod 700 /var/www/awakener/awakener
```

The process runs via `supervisorctl` with `awakener.conf` config. If changed, upload it to `/etc/supervisor/conf.
d/awakener.conf` and then:

```shell
sudo supervisorctl reread
sudo supervisorctl restart awakener
```

Even tho it's a standalone web server it runs behind lighttpd and the config is `lighttpd.conf`. Put its content 
into `/etc/lighttpd/external.conf`. Validate the config:

```shell
sudo /etc/init.d/lighttpd configtest
```

Restart lighttpd:

```shell
sudo /etc/init.d/lighttpd reload
```