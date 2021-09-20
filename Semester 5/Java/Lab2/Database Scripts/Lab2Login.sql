USE [master]
GO

/* For security reasons the login is created disabled and with a random password. */
/****** Object:  Login [Lab2Login]    Script Date: 9/20/2021 9:13:05 PM ******/
CREATE LOGIN [Lab2Login] WITH PASSWORD=N'r1zomm+nnPREEBkV/yGTPY1RYMUn6N7u42APXX3H90k=', DEFAULT_DATABASE=[master], DEFAULT_LANGUAGE=[us_english], CHECK_EXPIRATION=ON, CHECK_POLICY=ON
GO

ALTER LOGIN [Lab2Login] DISABLE
GO

ALTER SERVER ROLE [sysadmin] ADD MEMBER [Lab2Login]
GO

ALTER SERVER ROLE [securityadmin] ADD MEMBER [Lab2Login]
GO

ALTER SERVER ROLE [serveradmin] ADD MEMBER [Lab2Login]
GO

ALTER SERVER ROLE [setupadmin] ADD MEMBER [Lab2Login]
GO

ALTER SERVER ROLE [processadmin] ADD MEMBER [Lab2Login]
GO

ALTER SERVER ROLE [diskadmin] ADD MEMBER [Lab2Login]
GO

ALTER SERVER ROLE [dbcreator] ADD MEMBER [Lab2Login]
GO

ALTER SERVER ROLE [bulkadmin] ADD MEMBER [Lab2Login]
GO


