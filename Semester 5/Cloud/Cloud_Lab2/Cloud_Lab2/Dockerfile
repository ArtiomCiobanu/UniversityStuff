#See https://aka.ms/containerfastmode to understand how Visual Studio uses this Dockerfile to build your images for faster debugging.

#Depending on the operating system of the host machines(s) that will build or run the containers, the image specified in the FROM statement may need to be changed.
#For more information, please see https://aka.ms/containercompat

FROM mcr.microsoft.com/dotnet/aspnet:5.0-alpine3.12 AS base
WORKDIR /app
EXPOSE 80
EXPOSE 12345

FROM mcr.microsoft.com/dotnet/sdk:5.0-alpine3.13 AS publish
WORKDIR /src
COPY . .
RUN dotnet publish "Cloud_Lab2.csproj" -c Release -o /app/publish

FROM base AS final
WORKDIR /app
COPY --from=publish /app/publish .
ENTRYPOINT dotnet Cloud_Lab2.dll

#FROM mcr.microsoft.com/dotnet/aspnet:5.0 AS build-env
#WORKDIR /app
#
#COPY *.csproj ./
#RUN dotnet restore
#
#COPY . ./
#RUN dotnet publish -c Release -o out
#
#FROM mcr.microsoft.com/dotnet/aspnet:5.0
#COPY --from=build-env /app/out .
#ENTRYPOINT ["dotnet", "Cloud_Lab2.dll" ]
#