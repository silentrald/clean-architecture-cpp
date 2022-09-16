
CREATE TABLE IF NOT EXISTS users (
  id        UUID          PRIMARY KEY DEFAULT uuid_generate_v4(),
  username  VARCHAR(50)   UNIQUE NOT NULL,
  password  VARCHAR(150)  NOT NULL
);

INSERT INTO users(username, password)
VALUES
  -- NOTE: PASSWORD=password
  ('username', '$argon2id$v=19$m=16348,t=2,p=1$c3VwZXItc2VjcmV0LXBhc3N3b3JkICMgQ2hhbmdlIHRoaXMgaW4gcHJvZHVjdGlvbg$U8In1f2rnB3zuCmCFW5abuaPS41MstWd9/u/5yIoIms');

DROP TABLE IF EXISTS users;

