# ネットワーク設定

## 無線LAN設定方法
[RaspberryPiの無線接続](../part1/part1_1/connect_raspberry_wireless.md)を参照してください．

## 有線LAN設定方法
**学内のネットワークに有線接続をする場合は必ずこの方法を用いてください．**

PC-RaspberryPi間の有線接続に関しては，[RaspberryPiの有線接続](../part1/part1_1/connect_raspberry_wired.md)を参照してください．

ここで，RaspberryPiのEthernetデバイスに固定IP（`192.168.1.101`）を設定していますが，学内のネットワークに有線接続をする場合，この設定を変更する必要があります．


**初期設定：有線接続用の WPA-Supplicant の作成**

`/etc/wpa_supplicant/wpa_supplicant_wired.conf` を作成する．

```bash
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1
country=JP

network={
        priority=1
        key_mgmt=IEEE8021X
        eap=PEAP
        identity="情報学部アカウント"
        password=hash:{パスワードのハッシュ値}
        phase2="auth=MSCHAPV2"
}
```

パスワードのハッシュ値は，WRL-SUCCESの接続設定と同様の手順で取得することができます．

**接続時**

`/etc/dhcpcd.conf` の eth0 固定IP関連設定をコメントアウトした上で，ネットワークを一度止めます．

```bash
systemctl stop networking
```

もし既存の eth0 設定があれば停止します．
```bash
# eth0 に設定されている IP アドレスをすべて削除
sudo ip a flush dev eth0
# eth0 に対応する wpa_supplicant を終了
sudo wpa_cli -i eth0 terminate
```

新しい設定で接続します．
```bash
# 作った設定で接続
sudo wpa_supplicant -B -i eth0 -c /etc/wpa_supplicant/wpa_supplicant_wired.conf -D wired
# IPアドレスの再取得
sudo dhclient eth0
```

**切断時**

```bash
sudo wpa_cli -i eth0 terminate
sudo dhclient -r eth0
```

接続時，切断時の処理をスクリプトにまとめておくと運用が楽でしょう．

もし，PC-RaspberryPi間の有線接続を再開したい場合には，`/etc/dhcpcd.conf` の固定IP関連のコメントを解除した上で，ネットワークを再起動します．

```bash
sudo systemctl restart networking
```

なお，情報学部1号館実験室で有線接続する場合，以下の図のように`PIP`と書かれた情報コンセントを使用してください．

```{image} ../../images/trouble_shooting/Ethernet_Outlet.jpg
:alt: 情報コンセントの写真
:width: 800px
:align: center
```

## 有線-無線の優先度変更方法

有線と無線のどちらが優先されているか確認します．

```bash
pi@raspberrypi:~ $ ip route show default
default via {eth0 のデフォルトゲートウェイ} dev eth0 proto dhcp src {eth0 の IP} metric 202 
default via {wlan0 のデフォルトゲートウェイ}  dev wlan0 proto dhcp src {wlan0 の IP}  metric 303 
```

metricの後ろの数字が小さいほど，接続が優先されます．
筆者の環境の場合，eth0の数字の方が小さいため，有線接続が優先されています．

設定を変更したい場合，`/etc/dhcpcd.conf` で metric を書き換えます．

例えば，無線接続を優先したい場合，
```bash
interface wlan0
metric 100

interface eth0
metric 300
```
をファイル末尾に追記してください．
