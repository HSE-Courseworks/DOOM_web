FROM nginx:latest

COPY nginx.conf /etc/nginx/conf.d/default.conf
COPY main.html /usr/share/nginx/html/
COPY main.js /usr/share/nginx/html/
COPY main.data /usr/share/nginx/html/
COPY main.wasm /usr/share/nginx/html/

EXPOSE 80