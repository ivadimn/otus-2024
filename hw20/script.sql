CREATE TABLE "param" (
    "id"	INTEGER PRIMARY KEY AUTOINCREMENT,
    "name"  TEXT NOT NULL,
    "value" TEXT  NOT NULL,
    "ex_value" TEXT, 
    "description" TEXT
);
CREATE UNIQUE INDEX "name_IDX" ON "param" ("name"); 

INSERT INTO param(name, value, ex_value, description) values('123456', '0000001', '00000002', 'description'); 
