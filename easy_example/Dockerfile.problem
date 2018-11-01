RUN apt-get update && apt-get -y upgrade
RUN apt install -y socat curl
RUN groupadd -r user && useradd -r -g user user

COPY file:b03dfdb9310a1b1abeb69d836bf6f426bd7a884a116e979731d07f15353b6f6d in /send_flag
WORKDIR /home/user/
USER user
COPY --chown=root:userdir:2c06f831548564b50690075c5f820005e1875fb579c30d637c48ad575cb7fade in .
EXPOSE 12345

CMD ["socat" "TCP-LISTEN:12345,reuseaddr" "EXEC:/home/user/easy_example"]
