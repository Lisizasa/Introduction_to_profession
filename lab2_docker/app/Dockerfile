FROM python:3.11-slim
USER root
WORKDIR /app
COPY requirements.txt ./
RUN apt-get update && \
    apt-get install -y python3-pip && \
    pip install --upgrade pip && \
    pip install --no-cache-dir \
    --index-url=https://pypi.tuna.tsinghua.edu.cn/simple \
    -r requirements.txt
COPY app.py ./
EXPOSE 5050
CMD ["python", "app.py"]
