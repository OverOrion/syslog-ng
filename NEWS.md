3.33.1
======

## Highlights

<Fill this block manually from the blocks below>

## Features

 * kafka-c: batching support in case of sync_send(yes)
   
   ```
   kafka-c(
    bootstrap-server("localhost:9092")
    topic("syslog-ng")
    sync_send(yes)
    batch_lines(10)
    btach_timeout(10000)
   );
   ```
   
   Note1: batch_lines are accepted in case of sync_send(no), but no batching is done.
   Note2: messages are still sent one at a time to kafka, the batch yields multiple message per transaction.
   ([#3699](https://github.com/syslog-ng/syslog-ng/pull/3699))
 * `discord()` destination
   
   syslog-ng now has a webhook-based Discord destination.
   Example usage:
   ```
   destination {
     discord(url("https://discord.com/api/webhooks/x/y"));
   };
   ```
   
   The following options can be used to customize the destination further:
   
   `avatar-url()`, `username("$HOST-bot")`, `tts(true)`, `template("${MSG:-[empty message]}")`.
   ([#3717](https://github.com/syslog-ng/syslog-ng/pull/3717))
 * kafka-c: sync_send(yes) enables synchronous message delivery, reducing the possibility of message loss.
   
   ```
   kafka-c(
     bootstrap-server("localhost:9092")
     topic("syslog-ng")
     sync_send(yes)
   );
   ```
   
   Warning: this option also reduces significantly the performance of kafka-c driver.
   ([#3681](https://github.com/syslog-ng/syslog-ng/pull/3681))

## Bugfixes

 * `syslog-format`: fixing the check-hostname(yes|no) option
   
   The check-hostname(yes|no) option detected every value as invalid, causing a parse error when enabled.
   ([#3690](https://github.com/syslog-ng/syslog-ng/pull/3690))
 * `disk-buffer()`: fix crash when switching between disk-based and memory queues
   
   When a disk-buffer was removed from the configuration and the new config was
   applied by reloading syslog-ng, a crash occurred.
   ([#3700](https://github.com/syslog-ng/syslog-ng/pull/3700))
 * `logpath`: Fixed a message write protection bug, where message modifications (rewrite rules, parsers, etc.)
   leaked through preceding path elements. This may have resulted not only in unwanted/undefined message modification,
   but in certain cases crash as well.
   ([#3708](https://github.com/syslog-ng/syslog-ng/pull/3708))
 * `mongodb()`: fix crash with older mongo-c-driver versions
   
   syslog-ng crashed (was aborted) when the `mongodb()` destination was used with
   older mongo-c-driver versions (< v1.11.0).
   ([#3677](https://github.com/syslog-ng/syslog-ng/pull/3677))
 * `java()`: fix debug logging of Java-based destinations
   
   Java debug logging was not enabled previously when syslog-ng was started in debug/trace mode. This has been fixed.
   ([#3679](https://github.com/syslog-ng/syslog-ng/pull/3679))
 * kafka-c: fixed a hang during shutdown/reload, when multiple workers is used (workers() option is set to 2 or higher) and the librdkafka internal queue is filled.
   (error message was `kafka: failed to publish message; topic='test-topic', error='Local: Queue full'`)
   ([#3711](https://github.com/syslog-ng/syslog-ng/pull/3711))

## Packaging

 * kafka: minimum version of librdkafka is changed from 1.0.0 to 1.1.0
   ([#3706](https://github.com/syslog-ng/syslog-ng/pull/3706))
 * configure: now supporting python with two digit minor version
   ([#3713](https://github.com/syslog-ng/syslog-ng/pull/3713))

## Other changes

 * kafka: removed some deprecated options: client-lib-dir(), option(), template(), kafka-bootstrap-servers()
   ([#3698](https://github.com/syslog-ng/syslog-ng/pull/3698))
 * kafka: properties-file() option is deprecated. Please list the librdkafka properties in the config() option in syslog-ng's configuration.
   ([#3698](https://github.com/syslog-ng/syslog-ng/pull/3698))
 * `smtp()`: libesmtp is now detected via pkg-config
   ([#3669](https://github.com/syslog-ng/syslog-ng/pull/3669))

## Credits

syslog-ng is developed as a community project, and as such it relies
on volunteers, to do the work necessarily to produce syslog-ng.

Reporting bugs, testing changes, writing code or simply providing
feedback are all important contributions, so please if you are a user
of syslog-ng, contribute.

We would like to thank the following people for their contribution:

Andras Mitzki, Antal Nemes, Attila Szakacs, Balazs Scheidler,
Balázs Barkó, Gabor Nagy, Laszlo Budai, Laszlo Szemere, LittleFish33,
László Várady, Norbert Takacs, Peter Kokai, Todd C. Miller,
Zoltan Pallagi
