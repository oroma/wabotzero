# phone < -- > ESP
## command set
### preset
- http://192.168.0.1/preset/act{0..10}

### manual
- http://192.168.0.1/command/joint?d={l,r}&no={0..3}&angle={0..180}
- http://192.168.0.1/command/wheel?d={f,b,l,r}&v=20

# ESP < -- > MEGA
## command set
- '{'로 시작
- '}'로 끝
- ';'로 명령어 구분
### preset
```
{preset;1}
```
preset 설정 중에 1번
```
{preset;4}
```
preset 설정 중에 4번
### manual
```
{joint;l;20;20;20}
```
joint 동작 명령, left joint, angle 20
