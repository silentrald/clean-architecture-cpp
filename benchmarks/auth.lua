wrk.method = "POST"
-- wrk.body   = '                   password       username    '
-- wrk.headers["Content-Type"] = "application/flatbuffers"
wrk.body   = '{"username": "username", "password": "password"}'
wrk.headers["Content-Type"] = "application/json"
-- wrk.headers["Cookie"] = "SESSION=asdfasdfasdfasdf"
