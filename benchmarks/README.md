# How to run benchmarking

Install `wrk` from this link: [github](https://github.com/wg/wrk)

After installation, just run the command

```
# Threads, Connections and Duration are adjustable.
wrk -t4 -c100 -d10s http://localhost:5000/api/<endpoint>
```

