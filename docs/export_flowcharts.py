"""
租书店管理系统 - 流程图导出脚本
直接使用 PlantUML 在线服务器 + 自定义编码
"""
import os
import sys
import zlib
import string
import requests

if sys.platform == 'win32':
    sys.stdout.reconfigure(encoding='utf-8')

OUTPUT_DIR = os.path.join(os.path.dirname(__file__), "..", "docs", "images")
os.makedirs(OUTPUT_DIR, exist_ok=True)

def encode6bit(b):
    if b < 10: return chr(48 + b)
    b -= 10
    if b < 26: return chr(65 + b)
    b -= 26
    if b < 26: return chr(97 + b)
    b -= 26
    if b == 0: return chr(45)
    if b == 1: return chr(95)
    return '?'

def append3bytes(b1, b2, b3):
    c1 = b1 >> 2
    c2 = ((b1 & 0x3) << 4) | (b2 >> 4)
    c3 = ((b2 & 0xF) << 2) | (b3 >> 6)
    c4 = b3 & 0x3F
    return (encode6bit(c1 & 0x3F) + encode6bit(c2 & 0x3F) +
            encode6bit(c3 & 0x3F) + encode6bit(c4 & 0x3F))

def encode_plantuml(text):
    data = text.encode('utf-8')
    compressed = zlib.compress(data)[2:-4]
    result = []
    i = 0
    while i < len(compressed):
        if i + 2 < len(compressed):
            result.append(append3bytes(compressed[i], compressed[i+1], compressed[i+2]))
        elif i + 1 < len(compressed):
            result.append(append3bytes(compressed[i], compressed[i+1], 0))
        else:
            result.append(append3bytes(compressed[i], 0, 0))
        i += 3
    return ''.join(result)

def render(puml_text, output_path, scale=4, fmt="svg"):
    encoded = encode_plantuml(puml_text)
    url = f'https://www.plantuml.com/plantuml/{fmt}/~{scale}0/{encoded}'
    resp = requests.get(url, timeout=30)
    if resp.status_code == 200 and len(resp.content) > 500:
        with open(output_path, 'wb') as f:
            f.write(resp.content)
        return True, len(resp.content)
    return False, resp.status_code

# ========== 流程图定义 ==========

diagrams = {}

diagrams["01_系统登录流程"] = """@startuml
skinparam backgroundColor #FEFEFE
start
:选择角色 (管理员/店员/会员);
:输入账号密码;
:读取 user.csv 验证;

if (验证通过?) then (是)
  :显示角色菜单;
  stop
else (否)
  :提示错误, 重新输入;
  stop
endif
@enduml"""

diagrams["02_图书租借流程"] = """@startuml
skinparam backgroundColor #FEFEFE
start
:输入会员账号和图书ID;

if (会员存在?) then (是)
  :验证图书库存;
  if (库存>0?) then (是)
    :生成租借记录 (ID=R+时间戳);
    :库存 -1;
    :计算应还日期 (租借日+30天);
    :保存到 rental.csv;
    stop
  else (否)
    :提示库存不足;
    stop
  endif
else (否)
  :提示会员不存在;
  stop
endif
@enduml"""

diagrams["03_图书归还流程"] = """@startuml
skinparam backgroundColor #FEFEFE
start
:输入租借记录ID;
:查找租借记录;

if (记录存在且未归还?) then (是)
  :设置归还日期 = 当前日期;
  :库存 +1;
  :计算租借费用 = 书价 x 1% x 天数;
  
  if (逾期>30天?) then (是)
    :计算逾期罚款 = 书价 x 2% x 逾期天数;
  else (否)
  endif
  
  :显示费用明细;
  :更新 rental.csv;
  stop
else (否)
  :提示记录无效;
  stop
endif
@enduml"""

diagrams["04_会员注册流程"] = """@startuml
skinparam backgroundColor #FEFEFE
start
:管理员选择添加会员;
:输入会员信息 (账号/密码/姓名/手机);

if (账号重复?) then (是)
  :提示重新输入;
  stop
else (否)
  :写入 member.csv;
  :自动生成登录账号 (role=3);
  :写入 user.csv;
  stop
endif
@enduml"""

diagrams["05_系统架构总览"] = """@startuml
skinparam backgroundColor #FEFEFE
skinparam rectangleBorderColor #333333

rectangle "表现层\\n(main.cpp)\\n菜单/登录/路由" as presentation #LightBlue
rectangle "业务逻辑层" as logic #LightGreen {
  rectangle "BookManager\\n图书CRUD" as bm
  rectangle "MemberManager\\n会员CRUD" as mm
  rectangle "RentalManager\\n租借/归还" as rm
  rectangle "DateUtil\\n日期计算" as du
}
rectangle "数据层\\n(CSV文件)" as data #LightYellow {
  rectangle "user.csv" as ucsv
  rectangle "book.csv" as bcsv
  rectangle "category.csv" as ccsv
  rectangle "member.csv" as mcsv
  rectangle "rental.csv" as rcsv
}

presentation --> bm : "图书管理"
presentation --> mm : "会员管理"
presentation --> rm : "租借操作"
bm --> bcsv
bm --> ccsv
mm --> mcsv
mm --> ucsv
rm --> rcsv
@enduml"""

diagrams["06_综合项目主流程"] = """@startuml
skinparam backgroundColor #FEFEFE
start
:启动系统;
:加载CSV数据到内存;
:选择角色并登录;

switch (角色?)
case (管理员)
  :图书管理(增删改查);
  :会员管理(增删改查);
  :代会员租借/归还;
  :保存数据;
case (店员)
  :代会员租借图书;
  :归还图书;
  :查看逾期;
case (会员)
  :查看图书列表;
  :自行租借/归还;
  :查看记录;
endswitch

if (继续操作?) then (是)
  :返回角色选择;
  stop
else (否)
  :退出系统;
  stop
endif
@enduml"""

# ========== 执行导出 ==========
if __name__ == "__main__":
    print("=" * 50)
    print("租书店管理系统 - 流程图导出")
    print("=" * 50)
    
    success = 0
    fail = 0
    
    for name, puml in diagrams.items():
        print(f"\n渲染: {name}")
        output_file = os.path.join(OUTPUT_DIR, f"{name}.svg")
        try:
            ok, info = render(puml, output_file)
            if ok:
                print(f"  OK -> {output_file} ({info} bytes)")
                success += 1
            else:
                print(f"  FAIL -> HTTP {info}")
                fail += 1
        except Exception as e:
            print(f"  FAIL -> {e}")
            fail += 1
    
    print("\n" + "=" * 50)
    print(f"完成! 成功: {success}, 失败: {fail}")
    print(f"图片目录: {os.path.abspath(OUTPUT_DIR)}")
    print("=" * 50)
