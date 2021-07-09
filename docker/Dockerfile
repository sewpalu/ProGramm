FROM ubuntu

RUN apt-get update -y && apt-get install -y x11vnc xvfb libgtk-3-0

RUN echo "exec sh -c '/root/pro_gramm gui &>/tmp/pro_gramm.log'" > ~/.xinitrc && chmod +x ~/.xinitrc

COPY pro_gramm /root/pro_gramm

WORKDIR /root

CMD ["x11vnc", "-create", "-forever"]

