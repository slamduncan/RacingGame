uniform sampler2D sceneTexture;

// size of the final image, should be the same as sceneTexture
uniform float rt_w;	// render target width
uniform float rt_h;	// render target height

// probably do not need this, as it's going to be a FULL screen blur
uniform float vx_offset;	//amount of offset for the gaussian, vx_offset = 1.0

// precomputed offsets and weights for gaussian distribution
float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );

void main()
{
	vec3 tc = vec3(1.0, 0.0, 0.0);
	if (gl_TexCoord[0].x<(vx_offset-0.01))
	{
		vec2 uv = gl_TexCoord[0].xy;
		tc = texture2D(sceneTexture, uv).rgb * weight[0];
		for (int i=1; i<3; i++)
		{
			tc += texture2D(sceneTexture, uv + vec2(0.0, offset[i])/rt_h).rgb \* weight[i];
			tc += texture2D(sceneTexture, uv - vec2(0.0, offset[i])/rt_h).rgb \* weight[i];
		}
	}
	else if (gl_TexCoord[0].x>=(vx_offset+0.01))
	{
		tc = texture2D(sceneTexture, gl_TexCoord[0].xy).rgb;
	}
	gl_FragColor = vec4(tc, 1.0);
}